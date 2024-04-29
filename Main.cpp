#include "Lost.h"
#include "World.h"
#include "TileManager.h"
#include "Generator.h"
#include "Player.h"

#define SOKOL_IMPL
#define SOKOL_IMGUI_IMPL
#define SOKOL_GLCORE33
#define SOKOL_DEBUG
#include "SokolReference.h"
#include "STBImplementation.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <filesystem>

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

	lost::startProcessTime();

	sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);

	g_World->update(lost::globalCamera.getViewBounds());
	lost::calcProcessTime("Update Time");

	lost::globalCamera.update(lost::deltaTime);
	lost::globalCamera.setViewportTransforms();

	g_World->render(lost::globalCamera.getViewBounds());

	lost::Vector2D worldMouse = lost::globalCamera.screenToWorld(lost::mousePos());
	lost::Vector2D blockMouse = { floor(worldMouse.x / 32.0f), floor(worldMouse.y / 32.0f) };

	Tile* tileHovered = g_World->getTileAt(floor(worldMouse.x / 32.0f), floor(worldMouse.y / 32.0f));
	if (tileHovered)
	{
		if (!ImGui::IsAnyItemActive())
		{
			lost::useImage(1);
			float imageWidth = lost::getImage(1)->width;
			float imageHeight = lost::getImage(1)->height;
			bool CanPlace = g_World->checkStable({ blockMouse.x, blockMouse.y + 8, 3.0f, 1.0f }) && g_World->checkCanPlace({ blockMouse.x, blockMouse.y, 3.0f, 8.0f }, { false, true, false });
			
			if (CanPlace)
				sgp_set_color(1.0f, 1.0f, 1.0f, 0.2f);
			else
				sgp_set_color(1.0f, 0.1f, 0.1f, 0.2f);

			sgp_draw_textured_rect(0, { (float)blockMouse.x * 32.0f, (float)blockMouse.y * 32.0f, 3.0f * 32.0f, 8.0f * 32.0f }, { 0, 0, imageWidth, imageHeight });
			if (lost::mouseDown(0) && CanPlace)
				g_World->addTileEntity(new TileEntity(g_TileManager.getTileEntityRef("petrifiedTree")), floor(worldMouse.x / 32.0f), floor(worldMouse.y / 32.0f));
			lost::clearImage();
		}

		if (!ImGui::IsAnyItemActive() && lost::mouseTapped(2))
		{
			g_World->addEntity(new Entity({ worldMouse.x, worldMouse.y, 30.0f, 30.0f }));
		}

		for (int i = tileHovered->tileEntitiesWithin.size() - 1; i >= 0; i--)
		{
			sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);
			tileHovered->tileEntitiesWithin[i]->renderHitbox();
			if (!ImGui::IsAnyItemActive() && lost::mouseDown(1))
				g_World->destroyTileEntity(tileHovered->tileEntitiesWithin[i]);
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

	// Reads the files within the TileData folder and loads them if they end in .lua
	std::string tileDataPath = "GameData/TileData/";
	for (const auto& entry : std::filesystem::directory_iterator(tileDataPath))
	{
		std::string pathToString = entry.path().string();
		if (pathToString.substr(pathToString.size() - 4, 4) == ".lua")
			g_TileManager.loadTileData(pathToString.c_str());
	}

	// Reads the files within the TileEntityData folder and loads them if they end in .lua
	tileDataPath = "GameData/TileEntityData/";
	for (const auto& entry : std::filesystem::directory_iterator(tileDataPath))
	{
		std::string pathToString = entry.path().string();
		if (pathToString.substr(pathToString.size() - 4, 4) == ".lua")
			g_TileManager.loadTileEntityData(pathToString.c_str());
	}

	g_TileManager.createImageData();
	g_TileManager.createTileData();
	g_TileManager.createTileEntityData();
	g_TileManager.finishLoading();

	lost::loadImageQueue();

	g_World = new World();
	g_World->worldInit();
	g_World->addEntity(new Player({0, 0}));

	simgui_desc_t simguiSetupDesc = {};
	simgui_setup(&simguiSetupDesc);
}

// Called when the application is shutting down.
static void cleanup(void) {

	lost::destroyManagers();
	lost::globalCamera.unbindGoalTransform(&cameraGoalPos);
	
	delete g_World;

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