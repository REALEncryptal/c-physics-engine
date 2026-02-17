#pragma once

typedef struct Simulation {
    void (*init)(void);
    void (*physics)(float dt);
    void (*render)(void);
} Simulation;
