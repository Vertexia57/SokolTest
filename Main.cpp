#include "Lost.h"

#define SOKOL_IMPL
#define SOKOL_GLCORE33
#define SOKOL_DEBUG
#include "SokolReference.h"
#include "STBImplementation.h"

#include <stdio.h>
#include <stdlib.h>

struct effect_uniforms_t {
    sgp_vec2 iVelocity;
    float iPressure;
    float iTime;
    float iWarpiness;
    float iRatio;
    float iZoom;
    float iLevel;
};

static TextureID shmeldonTex = 0;
static TextureID noiseTex = 0;
static ShaderID effectShader = 0;
static float Uptime = 0;

static lost::Transform2D mousePos;

static void event_userdata_cb(const sapp_event* user_event, void* user_data)
{
    lost::feedKeyEvent(user_event);
}

static void frame()     
{

    // Get current window size.
    int width = sapp_width(), height = sapp_height();

    float window_ratio = (float)width / (float)height;
    float image_ratio = (float)lost::getImage(0)->height / (float)lost::getImage(0)->width;

    // Begin recording draw commands for a frame buffer of size (width, height).
    sgp_begin(width, height);
    // Set frame buffer drawing region to (0,0,width,height).
    sgp_viewport(0, 0, width, height);

    // Clear the frame buffer.
    sgp_set_color(0.1f, 0.1f, 0.1f, 1.0f);
    sgp_clear();

    lost::recalcDeltaTime();
    lost::globalCamera.setSize(width, height);
    lost::globalCamera.update(lost::deltaTime);
    lost::globalCamera.setViewportTransforms();

    lost::bindShader(lost::getShader(effectShader));

    Uptime += lost::deltaTime / 1000.0f;

    effect_uniforms_t uniforms = { };
    uniforms.iVelocity.x = 0.02f;
    uniforms.iVelocity.y = 0.01f;
    uniforms.iPressure = 0.3f;
    uniforms.iTime = Uptime;
    uniforms.iWarpiness = 0.2f;
    uniforms.iRatio = image_ratio;
    uniforms.iZoom = 0.4f;
    uniforms.iLevel = 1.0f;

    if (lost::keyDown(SAPP_KEYCODE_A))
        mousePos.position.x -= 10.0f;
    if (lost::keyDown(SAPP_KEYCODE_D))
        mousePos.position.x += 10.0f;
    if (lost::keyDown(SAPP_KEYCODE_W))
        mousePos.position.y -= 10.0f;
    if (lost::keyDown(SAPP_KEYCODE_S))
        mousePos.position.y += 10.0f;

    if (lost::keyTapped(SAPP_KEYCODE_E))
        mousePos.position.y += 100.0f;

    lost::resetKeyData();

    sgp_set_uniform(&uniforms, sizeof(effect_uniforms_t));

    // Draw an animated rectangle that rotates and changes its colors.
    sgp_set_color(1.0f, 1.0f, 1.0f, 1.0f);

    int i = 0;
    for (int x = 0; x < width * 2; x += 100)
    {
        for (int y = 0; y < height * 2; y += 100)
        {
            if (i % 2 == 0)
                lost::useImage(shmeldonTex, 0);
            else
                lost::useImage(noiseTex, 0);

            sgp_draw_filled_rect(x, y, -100.0f, 100.0f * image_ratio);

            i++;
        }
    }

    lost::unbindShader();

    // Begin a render pass.
    sg_pass pass = {}; 
    pass.swapchain = sglue_swapchain();

    sg_begin_pass(&pass);
    // Dispatch all draw commands to Sokol GFX.
    sgp_flush();
    // Finish a draw command queue, clearing it.
    sgp_end();
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
    sgp_setup(&sgpdesc);
    if (!sgp_is_valid()) {
        fprintf(stderr, "Failed to create Sokol GP context: %s\n", sgp_get_error_message(sgp_get_last_error()));
        exit(-1);
    }

    lost::createManagers();

    effectShader = lost::loadShader("Shaders/vertex.vert", "Shaders/fragment.frag", "EffectShader");

    shmeldonTex = lost::loadImage("Images/Shmeldon.png", "shmeldon");
    noiseTex = lost::loadImage("Images/perlin.png", "perlin");

    lost::globalCamera.bindGoalTransform(&mousePos, 0);
    lost::globalCamera.setSize(sapp_width(), sapp_height());

    lost::loadImageQueue();
}

// Called when the application is shutting down.
static void cleanup(void) {

    lost::destroyManagers();
    lost::globalCamera.unbindGoalTransform(&mousePos);

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
    sokolAppDescription.window_title = "Rectangle (Sokol GP)";
    sokolAppDescription.logger.func = slog_func;
    sokolAppDescription.win32_console_attach = true;
    sokolAppDescription.event_userdata_cb = event_userdata_cb;
    return sokolAppDescription;
}