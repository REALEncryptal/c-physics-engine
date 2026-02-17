#pragma once

typedef struct AppConfig {
    const int width; // const for now because im lazy
    const int height;
    const char* title;
} AppConfig;

void app_setup(
    AppConfig config, 
    void (*physics)(float dt), // run at a fixed timestep
    void (*render)(void) // runs right after update
);
