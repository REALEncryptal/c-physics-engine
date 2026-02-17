#include "raylib.h"
#include "app.h"
#include "sim/sim.h"

void app_setup(AppConfig config, Simulation *sim) {
    // init the simulation
    sim->init(&config);

    // init a window
    InitWindow(config.width, config.height, config.title);
    SetTargetFPS(60);

    // app loop
    const float FIXED_DT = 1.0f / 60.0f;
    float accumulator = 0.0f;

    while (!WindowShouldClose()) {
        float frame_time = GetFrameTime();
        accumulator += frame_time;

        while (accumulator >= FIXED_DT) {
            sim->physics(FIXED_DT);
            accumulator -= FIXED_DT;
        }

        BeginDrawing();
        ClearBackground(WHITE);
        sim->render();
        EndDrawing();
    }

    CloseWindow();
}