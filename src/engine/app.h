#pragma once

typedef struct AppConfig {
    const int width; // const for now because im lazy
    const int height;
    const char* title;
} AppConfig;

struct Simulation;
void app_setup(AppConfig config, struct Simulation *sim);
