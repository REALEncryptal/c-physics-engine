#pragma once

#include <math.h>

typedef struct {
    float x, y;
} Vec2;

// create a new Vec2
static inline Vec2 vec2(float x, float y) {
    return (Vec2){x, y};
}

// basic vector operations
static inline Vec2 vec2_add(Vec2 a, Vec2 b) {
    return (Vec2){a.x + b.x, a.y + b.y};
}

static inline Vec2 vec2_sub(Vec2 a, Vec2 b) {
    return (Vec2){a.x - b.x, a.y - b.y};
}

static inline Vec2 vec2_scale(Vec2 v, float s) {
    return (Vec2){v.x * s, v.y * s};
}

// dot product of two vectors
static inline float vec2_dot(Vec2 a, Vec2 b) {
    return a.x * b.x + a.y * b.y;
}

// length squared of a vector
static inline float vec2_len2(Vec2 v) {
    return vec2_dot(v, v);
}

// length of a vector (or magnitude)
static inline float vec2_len(Vec2 v) {
    return sqrtf(vec2_len2(v));
}

// get unit vector parallel to v
static inline Vec2 vec2_norm(Vec2 v) {
    float l = vec2_len(v);
    // if the len is tiny just return 0 -> prevent div by 0
    if (l < 1e-8f) return (Vec2){0, 0};
    return vec2_scale(v, 1.0f / l);
}

// distance between two vectors
static inline float vec2_dist(Vec2 a, Vec2 b) {
    return vec2_len(vec2_sub(a, b));
}

// rotate a vector by angle (radians, ccw), about the origin (0,0)
static inline Vec2 vec2_rotate(Vec2 v, float angle) {
    float c = cosf(angle);
    float s = sinf(angle);
    return vec2(v.x * c - v.y * s, v.x * s + v.y * c);
}

// world->local space (rel to origin, un-rotated)
static inline Vec2 vec2_world_to_local(Vec2 p, Vec2 origin, float rotation) {
    return vec2_rotate(vec2_sub(p, origin), -rotation);
}

// local->world space (rel to origin, rotated)
static inline Vec2 vec2_local_to_world(Vec2 p, Vec2 origin, float rotation) {
    return vec2_add(vec2_rotate(p, rotation), origin);
}

