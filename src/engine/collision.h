#pragma once

#include "stdbool.h"
#include "vec2.h"
#include "primitives.h"

#define COLLISION_EPSILON 1e-6f // prevent floating point probelms

// circle vs else
static inline bool circle_vs_circle(struct Circle a, struct Circle b) {
    float dist2 = vec2_len2(vec2_sub(a.origin, b.origin));
    float radii = a.radius + b.radius;
    return dist2 <= radii * radii + COLLISION_EPSILON;
}

static inline bool circle_vs_point(struct Circle c, Vec2 p) {
    float dist = vec2_dist(c.origin, p);
    return dist <= c.radius + COLLISION_EPSILON;
}

static inline bool circle_vs_square(struct Circle c, struct Square s) {
    // work in the square's local space so it becomes an AABB check
    Vec2 local = vec2_world_to_local(c.origin, s.origin, s.rotation);

    float hw = s.width  * 0.5f;
    float hh = s.height * 0.5f;

    // clamp circle center to the AABB to find closest point
    float cx = local.x;
    float cy = local.y;
    if (cx < -hw) cx = -hw;
    if (cx >  hw) cx =  hw;
    if (cy < -hh) cy = -hh;
    if (cy >  hh) cy =  hh;

    float dx = local.x - cx;
    float dy = local.y - cy;
    return dx * dx + dy * dy <= c.radius * c.radius + COLLISION_EPSILON;
}

// square vs else
static inline bool square_vs_point(struct Square s, Vec2 p) {
    // convert so that we can just do an AABB check.
    Vec2 local = vec2_world_to_local(p, s.origin, s.rotation);

    float hw = s.width  * 0.5f;
    float hh = s.height * 0.5f;

    // 4 checks arent need bcz origin is 0,0.
    return fabsf(local.x) <= hw + COLLISION_EPSILON
        && fabsf(local.y) <= hh + COLLISION_EPSILON;
}

// get world space corner
static inline void square_get_world_corners(struct Square s, Vec2 out[4]) {
    square_get_corners(s, out);
    for (int i = 0; i < 4; i++)
        out[i] = vec2_add(out[i], s.origin);
}

// SAT
static inline bool sat_axis_overlaps(Vec2 a[4], Vec2 b[4], Vec2 axis) {
    float a_min =  INFINITY, a_max = -INFINITY;
    float b_min =  INFINITY, b_max = -INFINITY;

    for (int i = 0; i < 4; i++) {
        float pa = vec2_dot(a[i], axis);
        float pb = vec2_dot(b[i], axis);
        if (pa < a_min) a_min = pa;
        if (pa > a_max) a_max = pa;
        if (pb < b_min) b_min = pb;
        if (pb > b_max) b_max = pb;
    }

    return a_max >= b_min - COLLISION_EPSILON
        && b_max >= a_min - COLLISION_EPSILON;
}

static inline bool square_vs_square(struct Square a, struct Square b) {
    // if both rots are about the sam then just do aabb
    if (fabsf(a.rotation - b.rotation) < COLLISION_EPSILON) {
        Vec2 d = vec2_world_to_local(b.origin, a.origin, a.rotation);
        float hw = (a.width + b.width) * 0.5f;
        float hh = (a.height + b.height) * 0.5f;
        return fabsf(d.x) <= hw + COLLISION_EPSILON
            && fabsf(d.y) <= hh + COLLISION_EPSILON;
    }

    Vec2 ca[4], cb[4];
    square_get_world_corners(a, ca);
    square_get_world_corners(b, cb);


    Vec2 axes[4] = {
        vec2( cosf(a.rotation), sinf(a.rotation)),
        vec2(-sinf(a.rotation), cosf(a.rotation)),
        vec2( cosf(b.rotation), sinf(b.rotation)),
        vec2(-sinf(b.rotation), cosf(b.rotation)),
    };

    for (int i = 0; i < 4; i++) {
        if (!sat_axis_overlaps(ca, cb, axes[i]))
            return false;
    }

    return true;
}

// line vs else
float orientation(Vec2 a, Vec2 b, Vec2 c) {
    Vec2 ab = vec2_sub(b, a);
    Vec2 ac = vec2_sub(c, a);
    return ab.x * ac.y - ab.y * ac.x;
}

static inline bool on_segment(Vec2 a, Vec2 b, Vec2 p) {
    return fminf(a.x, b.x) - COLLISION_EPSILON <= p.x && p.x <= fmaxf(a.x, b.x) + COLLISION_EPSILON
        && fminf(a.y, b.y) - COLLISION_EPSILON <= p.y && p.y <= fmaxf(a.y, b.y) + COLLISION_EPSILON;
}

bool line_vs_line(struct Line a, struct Line b) {
    Vec2 d1 = vec2_sub(a.end, a.start);
    Vec2 d2 = vec2_sub(b.end, b.start);
    float cross = d1.x * d2.y - d1.y * d2.x;

    if (fabsf(cross) < COLLISION_EPSILON) {
        // parallel/collinear — check endpoint overlap
        if (on_segment(a.start, a.end, b.start)) return true;
        if (on_segment(a.start, a.end, b.end))   return true;
        if (on_segment(b.start, b.end, a.start)) return true;
        if (on_segment(b.start, b.end, a.end))   return true;
        return false;
    }

    Vec2 d = vec2_sub(b.start, a.start);
    float t = (d.x * d2.y - d.y * d2.x) / cross;
    float u = (d.x * d1.y - d.y * d1.x) / cross;

    return t >= -COLLISION_EPSILON && t <= 1.0f + COLLISION_EPSILON
        && u >= -COLLISION_EPSILON && u <= 1.0f + COLLISION_EPSILON;
}

bool line_vs_circle(struct Line l, struct Circle c) {
    Vec2 ab = vec2_sub(l.end, l.start);
    Vec2 ac = vec2_sub(c.origin, l.start);

    float ab_len2 = vec2_len2(ab);

    // degenerate line (start == end), just do point vs circle
    if (ab_len2 < COLLISION_EPSILON)
        return circle_vs_point(c, l.start);

    // project center onto line, clamp t to segment
    float t = vec2_dot(ac, ab) / ab_len2;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    Vec2 closest = vec2_add(l.start, vec2_scale(ab, t));
    float dist2 = vec2_len2(vec2_sub(c.origin, closest));
    return dist2 <= c.radius * c.radius + COLLISION_EPSILON;
}

bool line_vs_square(struct Line l, struct Square s) {
    // either endpoint inside the square
    if (square_vs_point(s, l.start) || square_vs_point(s, l.end))
        return true;

    // line crosses any of the 4 edgesx
    Vec2 corners[4];
    square_get_world_corners(s, corners);

    for (int i = 0; i < 4; i++) {
        struct Line edge = { corners[i], corners[(i + 1) % 4] };
        if (line_vs_line(l, edge))
            return true;
    }

    return false;
}