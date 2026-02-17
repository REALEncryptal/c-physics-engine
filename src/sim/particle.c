#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "raylib.h"
#include "particle.h"
#include "engine/app.h"
#include "engine/vec2.h"
#include "engine/primitives.h"
#include "engine/collision.h"
#include "engine/particle.h"

static const AppConfig *config;
static Vec2 center;

const int NUM_PARTICLES = 200;
const float G = 0.1f;
const float TARGET_DIST = 120.0f;
const float INTERACT_RADIUS = 10000.0f;
const float RANDOM_OFFSET = 100.0f;
struct Particle *particles;

// util
float randomFloatRange(float min, float max) {
    float scale = (float)rand() / (float)RAND_MAX; // Range [0.0, 1.0]
    return min + scale * (max - min);             // Range [min, max]
}

// main
static void init(const AppConfig *cfg) {
    config = cfg;
    center = vec2((cfg->width)/2, (cfg->height)/2);

    // seed rng to current time
    srand(time(NULL));

    //alloc mem for particles then populate in an evenly spaced grid
    particles = malloc(NUM_PARTICLES * sizeof(struct Particle));

    int cols = (int)ceilf(sqrtf((float)NUM_PARTICLES));
    int rows = (int)ceilf((float)NUM_PARTICLES / cols);
    float spacing_x = (float)cfg->width / (cols + 1);
    float spacing_y = (float)cfg->height / (rows + 1);

    for (int i = 0; i < NUM_PARTICLES; i++) {
        int col = i % cols;
        int row = i / cols;
        particles[i] = (struct Particle){
            .position = vec2(
                spacing_x * (col + 1) + randomFloatRange(-RANDOM_OFFSET, RANDOM_OFFSET),
                spacing_y * (row + 1) + randomFloatRange(-RANDOM_OFFSET, RANDOM_OFFSET)
            ),
            .linear_velocity = vec2(0, 0),
            .mass = 1.0f,
            .color = BLACK,
            .radius = 2.0f
        };
    }
}

static void physics(float dt) {
    // pairwise gravity (only within interaction radius)
    for (int i = 0; i < NUM_PARTICLES; i++) {
        for (int j = i + 1; j < NUM_PARTICLES; j++) {
            float dist = vec2_dist(particles[i].position, particles[j].position);
            if (dist > INTERACT_RADIUS) continue;
            Vec2 dir = vec2_norm(vec2_sub(particles[j].position, particles[i].position));
            float force = (dist - TARGET_DIST) * G;
            Vec2 f = vec2_scale(dir, force);
            particle_force(&particles[i], f);
            particle_force(&particles[j], vec2_scale(f, 0.0f));
        }
    }

    for (int i = 0; i < NUM_PARTICLES; i++) {
        struct Particle *p = &particles[i];

        particle_attract(&particles[i], center, 10 * vec2_dist(particles[i].position, center));
        particle_drag(&particles[i], 0.2f);
        particle_update(p, dt);
        //particle_draw_arrow(p, vec2_len(p->linear_velocity));
        //particle_wrap_screen(p, config);
    }
}

static void render(void) {
    for (int i = 0; i<NUM_PARTICLES; i++) {
        particle_render(&particles[i]);
    }
}

Simulation particle_sim(void) {
    return (Simulation){init, physics, render};
}
