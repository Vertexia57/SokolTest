#include "Lost.h"
#include "World.h"
#include "TileManager.h"
#include "Generator.h"

#define SOKOL_IMPL
#define SOKOL_IMGUI_IMPL
#define SOKOL_GLCORE33
#define SOKOL_DEBUG
#include "SokolReference.h"
#include "STBImplementation.h"

#include <stdio.h>
#include <stdlib.h>

static ShaderID effectShader = 0;
static TextureID grassTex = 0;
static float Uptime = 0;

static lost::Transform2D cameraGoalPos;

static void event_userdata_cb(const sapp_event* user_event, void* user_data)
{
	simgui_handle_event(user_event);
	lost::feedInputEvent(user_event);
}

static void frame()     
{
	lost::recalcDeltaTime();

	// Get current window size.
	int width = sapp_width(), height = sapp_height();

	float window_ratio = (float)width / (float)height;
	float image_ratio = (float)lost::getImage(0)->height / (float)lost::getImage(0)->width;

	simgui_frame_desc_t imguiFrameData = {};
	imguiFrameData.width = width;
	imguiFrameData.height = height;
	imguiFrameData.delta_time = lost::deltaTime / 1000.0f;
	imguiFrameData.dpi_scale = 1;
	simgui_new_frame(&imguiFrameData);

	// Begin recording draw commands for a frame buffer of size (width, height).
	sgp_begin(width, height);
	// Set frame buffer drawing region to (0,0,width,height).
	sgp_viewport(0, 0, width, height);

	// Clear the frame buffer.
	sgp_set_color(0.1f, 0.1f, 0.1f, 1.0f);
	sgp_clear();

	Uptime += lost::deltaTime;
	lost::globalCamera.setSize(width, height);
	float scaleVal = fminf(fmaxf(cameraGoalPos.scale.x * (1.0f - lost::mouseScroll() / 10.0f), 0.5), 2.1f);
	cameraGoalPos.scale = { scaleVal, scaleVal };
	lost::globalCamera.update(lost::deltaTime);
	lost::globalCamera.setViewportTransforms();

	if (!ImGui::IsAnyItemActive())
	{
		if (lost::keyDown(SAPP_KEYCODE_A))
			cameraGoalPos.position.x -= 5.0f;
		if (lost::keyDown(SAPP_KEYCODE_D))
			cameraGoalPos.position.x += 5.0f;
		if (lost::keyDown(SAPP_KEYCODE_W))
			cameraGoalPos.position.y -= 5.0f;
		if (lost::keyDown(SAPP_KEYCODE_S))
			cameraGoalPos.position.y += 5.0f;
	}

	lost::startProcessTime();

	if (ImGui::Button("Generate Chunk +X"))
	{
		world->worldMaxX++;
		world->createChunk(world->worldMaxX);
	}

	if (ImGui::Button("Generate Chunk -X"))
	{
		world->worldMinX--;
		world->createChunk(world->worldMinX);
	}

	sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);

	world->update(lost::globalCamera.getViewBounds());
	lost::calcProcessTime("Update Time");
	world->render(lost::globalCamera.getViewBounds());

	lost::Vector2D worldMouse = lost::globalCamera.screenToWorld(lost::mousePos());
	lost::Vector2D blockMouse = { floor(worldMouse.x / 32.0f), floor(worldMouse.y / 32.0f) };

	Tile* tileHovered = world->getTileAt(floor(worldMouse.x / 32.0f), floor(worldMouse.y / 32.0f));
	if (tileHovered)
	{
		if (!ImGui::IsAnyItemActive())
		{
			lost::useImage(1);
			float imageWidth = lost::getImage(1)->width;
			float imageHeight = lost::getImage(1)->height;
			bool CanPlace = world->checkStable({ blockMouse.x, blockMouse.y + 8, 0.5f, 1.0f }) && world->checkCanPlace({ blockMouse.x, blockMouse.y, 0.5f, 8.0f }, { true, false, false });
			
			if (CanPlace)
				sgp_set_color(1.0f, 1.0f, 1.0f, 0.2f);
			else
				sgp_set_color(1.0f, 0.1f, 0.1f, 0.2f);

			sgp_draw_textured_rect(0, { (float)blockMouse.x * 32.0f, (float)blockMouse.y * 32.0f, 0.5f * 32.0f, 8.0f * 32.0f }, { 0, 0, imageWidth, imageHeight });
			if (lost::mouseDown(0) && CanPlace)
				world->addTileEntity(new TileEntity(1, { 0.0f, 0.0f, 0.5f, 7.8f }), floor(worldMouse.x / 32.0f) + 0.1f, floor(worldMouse.y / 32.0f) + 0.2f);
			lost::clearImage();
		}

		if (!ImGui::IsAnyItemActive() && lost::mouseTapped(2))
		{
			world->addEntity(new Entity({ worldMouse.x, worldMouse.y, 30.0f, 30.0f }));
		}

		for (int i = tileHovered->tileEntitiesWithin.size() - 1; i >= 0; i--)
		{
			if (tileHovered->tileEntitiesWithin[i]->getHitbox().inBounds(worldMouse / 32.0f))
			{
				sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);
				tileHovered->tileEntitiesWithin[i]->renderHitbox();
				if (!ImGui::IsAnyItemActive() && lost::mouseDown(1))
					world->destroyTileEntity(tileHovered->tileEntitiesWithin[i]);
			}
		}
	}

	lost::resetInputData();

	// Begin a render pass.
	sg_pass pass = {}; 
	pass.swapchain = sglue_swapchain();

	sg_begin_pass(&pass);   
	// Dispatch all draw commands to Sokol GFX.
	sgp_flush();
	// Finish a draw command queue, clearing it.
	sgp_end();

	lost::calcProcessTime("Render Time");
	// Display imgui
	simgui_render();

	// End render pass.
	sg_end_pass();
	// Commit Sokol render.
	sg_commit();
}

// Called when the application is initializing.
static void init(void) {
	// Initialize Sokol GFX.
	sg_desc sgdesc = {};
	sgdesc.environment = sglue_environment();
	sgdesc.logger.func = slog_func;

	sg_setup(&sgdesc);
	if (!sg_isvalid()) {
		fprintf(stderr, "Failed to create Sokol GFX context!\n");
		exit(-1);
	}

	// Initialize Sokol GP, adjust the size of command buffers for your own use.
	sgp_desc sgpdesc = { 0 };

	sgpdesc.max_vertices = (1 << 18);
	sgp_setup(&sgpdesc);
	if (!sgp_is_valid()) {
		fprintf(stderr, "Failed to create Sokol GP context: %s\n", sgp_get_error_message(sgp_get_last_error()));
		exit(-1);
	}

	lost::createManagers();

	effectShader = lost::loadShader("Shaders/vertex.vert", "Shaders/fragment.frag", "EffectShader");

	grassTex = lost::loadImage("Images/TestTile.png", "testTile");
	lost::loadImage("Images/Test.png", "test");

	lost::globalCamera.bindGoalTransform(&cameraGoalPos, 0);
	lost::globalCamera.setSize(sapp_width(), sapp_height());

	g_TileManager.loadTileData("TileData/Stone.json");
	g_TileManager.loadTileData("TileData/Air.json");
	
	lost::loadImageQueue();

	world = new World();

	world->worldInit();
	world->createChunk(0);

	simgui_desc_t simguiSetupDesc = {};
	simgui_setup(&simguiSetupDesc);
}

// Called when the application is shutting down.
static void cleanup(void) {

	lost::destroyManagers();
	lost::globalCamera.unbindGoalTransform(&cameraGoalPos);
	
	delete world;

	// Close Imgui
	simgui_shutdown();

	// Cleanup Sokol GP and Sokol GFX resources.
	sgp_shutdown();
	sg_shutdown();
}

// Implement application main through Sokol APP.
sapp_desc sokol_main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
	sapp_desc sokolAppDescription = {};
	sokolAppDescription.init_cb = init;
	sokolAppDescription.frame_cb = frame;
	sokolAppDescription.cleanup_cb = cleanup;
	sokolAppDescription.window_title = "TestGame";
	sokolAppDescription.logger.func = slog_func;
	sokolAppDescription.win32_console_attach = true;
	sokolAppDescription.event_userdata_cb = event_userdata_cb;
	return sokolAppDescription;
}