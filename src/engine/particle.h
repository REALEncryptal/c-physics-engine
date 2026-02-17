#pragma once

#include "raylib.h"
#include "vec2.h"
#include "primitives.h"
#include "app.h"

// not planning on do physics simulation, more just particle stuff.
struct Particle {
    Vec2 position;
    Vec2 linear_velocity;
    float mass;
    Color color;
    float radius;
};

// render
static inline void particle_render(struct Particle *p) {
    DrawCircleV(
        (Vector2){p->position.x, p->position.y},
        p->radius,
        p->color
    );
}


// apply a raw force: a = F / m
static inline void particle_force(struct Particle *p, Vec2 f) {
    Vec2 accel = vec2_scale(f, 1.0f / p->mass);
    p->linear_velocity = vec2_add(p->linear_velocity, accel);
}

// constant gravity
static inline void particle_gravity(struct Particle *p, Vec2 g) {
    Vec2 f = vec2_scale(g, p->mass);
    particle_force(p, f);
}

// linear drag
static inline void particle_drag(struct Particle *p, float coefficient) {
    Vec2 f = vec2_scale(p->linear_velocity, -coefficient);
    particle_force(p, f);
}

// newtonian gravity toward a target distance from a point
// attracts when farther than target_dist, repels when closer
static inline void particle_gravity_point(struct Particle *p, Vec2 point, float point_mass, float G, float target_dist) {
    Vec2 dir = vec2_sub(point, p->position);
    float dist = vec2_len(dir);
    if (dist < 1e-4f) return;
    Vec2 norm = vec2_scale(dir, 1.0f / dist);
    float offset = dist - target_dist;
    float magnitude = G * p->mass * point_mass * offset / (dist * dist);
    particle_force(p, vec2_scale(norm, magnitude));
}

// point attractor
static inline void particle_attract(struct Particle *p, Vec2 point, float strength) {
    Vec2 dir = vec2_sub(point, p->position);
    float dist = vec2_len(dir);
    if (dist < 1e-4f) return;
    Vec2 f = vec2_scale(vec2_norm(dir), strength / dist);
    particle_force(p, f);
}

// point repulsor
static inline void particle_repel(struct Particle *p, Vec2 point, float strength) {
    particle_attract(p, point, -strength);
}

// wrap particle to opposite side of screen
static inline void particle_wrap_screen(struct Particle *p, const AppConfig *cfg) {
    if (p->position.x < -p->radius) p->position.x = cfg->width + p->radius;
    if (p->position.x > cfg->width + p->radius) p->position.x = -p->radius;
    if (p->position.y < -p->radius) p->position.y = cfg->height + p->radius;
    if (p->position.y > cfg->height + p->radius) p->position.y = -p->radius;
}

// draw arrow in direction of velocity
static inline void particle_draw_arrow(struct Particle *p, float length) {
    Vec2 dir = vec2_norm(p->linear_velocity);
    Vec2 tip = vec2_add(p->position, vec2_scale(dir, length));

    // shaft
    DrawLineV(
        (Vector2){p->position.x, p->position.y},
        (Vector2){tip.x, tip.y},
        p->color
    );

    // arrowhead wings
    Vec2 back = vec2_scale(dir, -length * 0.25f);
    Vec2 left = vec2_add(tip, vec2_rotate(back, 0.4f));
    Vec2 right = vec2_add(tip, vec2_rotate(back, -0.4f));
    DrawTriangle(
        (Vector2){tip.x, tip.y},
        (Vector2){right.x, right.y},
        (Vector2){left.x, left.y},
        p->color
    );
}

// set position from velocity
static inline void particle_update(struct Particle *p, float dt) {
    p->position = vec2_add(p->position, vec2_scale(p->linear_velocity, dt));
}
