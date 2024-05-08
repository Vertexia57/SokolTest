#include "Lost.h"
#include "World.h"
#include "TileManager.h"
#include "ItemManager.h"
#include "Generator.h"
#include "ItemEntity.h"
#include "Player.h"
#include "StorageWindow.h"
#include "HUDWindow.h"

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

// [?] I don't feel like there should be an inventory in all honesty
// [?] I think buildings should just cost money, which you get at the start of the game
// [?] So I guess:
// [!] TODO: Add building costs, add player money and create a UI for it. Add a building menu and figure that out
// [!]       FOLLOW THE STUFF YOU WROTE!!!! Also make demo art.

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

	if (lost::keyDown(SAPP_KEYCODE_LEFT_CONTROL) && lost::keyTapped(SAPP_KEYCODE_R))
		lost::reloadImages();

	sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);

	g_World->update(lost::globalCamera.getViewBounds());
	lost::calcProcessTime("Update Time");

	lost::globalCamera.update(lost::deltaTime);
	lost::globalCamera.setViewportTransforms();

	lost::bindShader(lost::getShader(0));

	g_World->render(lost::globalCamera.getViewBounds());
	sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);

	lost::updateUI();
	lost::globalCamera.resetViewportTransforms();
	lost::renderUI();

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

	lua_State* loaderState = luaL_newstate();
	luaL_openlibs(loaderState);
	luaBindDebugPrint(loaderState);
	checkLua(loaderState, luaL_dostring(loaderState, read_text_file("GameData/dataBase.lua").c_str()));

	// Reads the files within the TileData folder and loads them if they end in .lua
	std::string tileDataPath = "GameData/TileData/";
	for (const auto& entry : std::filesystem::directory_iterator(tileDataPath))
	{
		std::string pathToString = entry.path().string();
		if (pathToString.substr(pathToString.size() - 4, 4) == ".lua")
			g_TileManager.loadTileData(loaderState, pathToString.c_str());
	}

	// Reads the files within the TileEntityData folder and loads them if they end in .lua
	tileDataPath = "GameData/TileEntityData/";
	for (const auto& entry : std::filesystem::directory_iterator(tileDataPath))
	{
		std::string pathToString = entry.path().string();
		if (pathToString.substr(pathToString.size() - 4, 4) == ".lua")
			g_TileManager.loadTileEntityData(loaderState, pathToString.c_str());
	}

	// Reads the files within the ItemData folder and loads them if they end in .lua
	tileDataPath = "GameData/ItemData/";
	for (const auto& entry : std::filesystem::directory_iterator(tileDataPath))
	{
		std::string pathToString = entry.path().string();
		if (pathToString.substr(pathToString.size() - 4, 4) == ".lua")
			g_ItemManager.loadItemData(loaderState, pathToString.c_str());
	}

	g_TileManager.createImageData(loaderState);
	g_TileManager.createTileData(loaderState);
	g_TileManager.createTileEntityData(loaderState);
	g_ItemManager.createItemData(loaderState);

	lua_close(loaderState);

	lost::loadImage("GameData/NBoxBase.png", "NBoxBase");
	lost::loadImage("GameData/SlotNBox.png", "SlotNBox");
	lost::loadImage("GameData/HUDTopLeft.png", "HUDTopLeft");
	lost::loadImage("GameData/Credit.png", "Credit");
	lost::loadImageQueue();

	g_World = new World();
	g_World->worldInit();
	g_PlayerPointer = new Player({ 0, 0 });
	g_World->addEntity(g_PlayerPointer);

	simgui_desc_t simguiSetupDesc = {};
	simgui_setup(&simguiSetupDesc);

	lost::loadFont("GameData/Fonts/PixeloidSans.ttf", 36, "Pixel");

	HUDWindow* windowH = new HUDWindow({ 0, 0, (float)sapp_width(), (float)sapp_height() });
	lost::addUIWindow(windowH);
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