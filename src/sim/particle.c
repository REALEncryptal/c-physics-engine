#include <stdlib.h>

#include "particle.h"
#include "engine/vec2.h"
#include "engine/primitives.h"
#include "engine/collision.h"

const int NUM_PARTICLES = 10;
struct Circle *particles;

static void init(void) {
    // alloc some space for the particles
    particles = malloc(NUM_PARTICLES * sizeof(struct Circle));

    // populate
    for (int i = 0; i<NUM_PARTICLES; i++) {
        particles[i] = (struct Circle){
            vec2(0,0),
            50.0f
        };
    }
}

static void physics(float dt) {
    (void)dt;
}

static void render(void) {
}

Simulation particle_sim(void) {
    return (Simulation){init, physics, render};
}
