#pragma once

#include <string.h>
#include "vec2.h"

struct Circle {
    Vec2 origin; // center
    float radius;
};

struct Square {
    Vec2 origin;
    float rotation; // must be in radians ccw
    float width, height;
};

struct Line {
    Vec2 start, end;
};

// TL, TR, BR, BL


/*

[x',y'] = [x,y] * [ cos(a) -sin(a)
                    sin(a)  cos(a) ]

x' = x * cos(a) - y * sin(a)
y' = x * sin(a) + y * cos(a)

*/
static inline void square_get_corners(struct Square s, Vec2 out[4]) {
    float hw = s.width  * 0.5f;
    float hh = s.height * 0.5f;

    // get local space corners
    Vec2 local[4] = {
        vec2(-hw, -hh), // TL
        vec2( hw, -hh), // TR
        vec2( hw,  hh), // BR
        vec2(-hw,  hh)  // BL
    };

    // if rotation is 0 then we can skip the rotation step and just return local
    if (s.rotation == 0.0f) {
        memcpy(out, local, sizeof(local));// copy local to out
        return;
    }

    // apply rotation to get world space corners
    for (int i = 0; i < 4; i++) {
        float x = local[i].x;
        float y = local[i].y;

        out[i] = vec2(
            x * cosf(s.rotation) - y * sinf(s.rotation),
            x * sinf(s.rotation) + y * cosf(s.rotation)
        );
    }
}
