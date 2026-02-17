#pragma once

#include "engine/app.h"

typedef struct Simulation {
    void (*init)(const AppConfig *config);
    void (*physics)(float dt);
    void (*render)(void);
} Simulation;
