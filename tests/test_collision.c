#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <math.h>

#include "engine/vec2.h"
#include "engine/primitives.h"
#include "engine/collision.h"

#define PI 3.14159265358979323846f

// ─── circle_vs_circle ────────────────────────────────────────────

static void test_circle_vs_circle_overlap(void) {
    struct Circle a = { vec2(0, 0), 5.0f };
    struct Circle b = { vec2(3, 0), 5.0f };
    CU_ASSERT_TRUE(circle_vs_circle(a, b));
}

static void test_circle_vs_circle_touching(void) {
    struct Circle a = { vec2(0, 0), 5.0f };
    struct Circle b = { vec2(10, 0), 5.0f };
    CU_ASSERT_TRUE(circle_vs_circle(a, b));
}

static void test_circle_vs_circle_apart(void) {
    struct Circle a = { vec2(0, 0), 5.0f };
    struct Circle b = { vec2(20, 0), 5.0f };
    CU_ASSERT_FALSE(circle_vs_circle(a, b));
}

static void test_circle_vs_circle_concentric(void) {
    struct Circle a = { vec2(1, 1), 10.0f };
    struct Circle b = { vec2(1, 1), 3.0f };
    CU_ASSERT_TRUE(circle_vs_circle(a, b));
}

static void test_circle_vs_circle_diagonal(void) {
    struct Circle a = { vec2(0, 0), 5.0f };
    struct Circle b = { vec2(3, 4), 5.0f };  // dist = 5, radii sum = 10
    CU_ASSERT_TRUE(circle_vs_circle(a, b));
}

// ─── circle_vs_point ─────────────────────────────────────────────

static void test_circle_vs_point_inside(void) {
    struct Circle c = { vec2(0, 0), 10.0f };
    CU_ASSERT_TRUE(circle_vs_point(c, vec2(3, 4)));
}

static void test_circle_vs_point_on_edge(void) {
    struct Circle c = { vec2(0, 0), 5.0f };
    CU_ASSERT_TRUE(circle_vs_point(c, vec2(5, 0)));
}

static void test_circle_vs_point_outside(void) {
    struct Circle c = { vec2(0, 0), 5.0f };
    CU_ASSERT_FALSE(circle_vs_point(c, vec2(6, 0)));
}

static void test_circle_vs_point_center(void) {
    struct Circle c = { vec2(5, 5), 1.0f };
    CU_ASSERT_TRUE(circle_vs_point(c, vec2(5, 5)));
}

// ─── circle_vs_square ────────────────────────────────────────────

static void test_circle_vs_square_overlap(void) {
    struct Circle c = { vec2(0, 0), 5.0f };
    struct Square s = { vec2(4, 0), 0.0f, 4.0f, 4.0f };
    CU_ASSERT_TRUE(circle_vs_square(c, s));
}

static void test_circle_vs_square_apart(void) {
    struct Circle c = { vec2(0, 0), 2.0f };
    struct Square s = { vec2(20, 0), 0.0f, 4.0f, 4.0f };
    CU_ASSERT_FALSE(circle_vs_square(c, s));
}

static void test_circle_vs_square_circle_inside(void) {
    struct Circle c = { vec2(5, 5), 1.0f };
    struct Square s = { vec2(5, 5), 0.0f, 20.0f, 20.0f };
    CU_ASSERT_TRUE(circle_vs_square(c, s));
}

static void test_circle_vs_square_touching_edge(void) {
    // square from x=5..15, circle center at 3 with radius 2 => touches at x=5
    struct Circle c = { vec2(3, 10), 2.0f };
    struct Square s = { vec2(10, 10), 0.0f, 10.0f, 10.0f };
    CU_ASSERT_TRUE(circle_vs_square(c, s));
}

static void test_circle_vs_square_rotated(void) {
    // 45-degree rotated square
    struct Circle c = { vec2(0, 0), 1.0f };
    struct Square s = { vec2(5, 0), PI / 4.0f, 4.0f, 4.0f };
    CU_ASSERT_FALSE(circle_vs_square(c, s));
}

static void test_circle_vs_square_corner_near_miss(void) {
    // circle near corner but not touching
    struct Circle c = { vec2(0, 0), 1.0f };
    struct Square s = { vec2(10, 10), 0.0f, 4.0f, 4.0f };
    CU_ASSERT_FALSE(circle_vs_square(c, s));
}

// ─── square_vs_point ─────────────────────────────────────────────

static void test_square_vs_point_inside(void) {
    struct Square s = { vec2(0, 0), 0.0f, 10.0f, 10.0f };
    CU_ASSERT_TRUE(square_vs_point(s, vec2(2, 3)));
}

static void test_square_vs_point_outside(void) {
    struct Square s = { vec2(0, 0), 0.0f, 10.0f, 10.0f };
    CU_ASSERT_FALSE(square_vs_point(s, vec2(20, 0)));
}

static void test_square_vs_point_on_edge(void) {
    struct Square s = { vec2(0, 0), 0.0f, 10.0f, 10.0f };
    CU_ASSERT_TRUE(square_vs_point(s, vec2(5, 0)));
}

static void test_square_vs_point_on_corner(void) {
    struct Square s = { vec2(0, 0), 0.0f, 10.0f, 10.0f };
    CU_ASSERT_TRUE(square_vs_point(s, vec2(5, 5)));
}

static void test_square_vs_point_rotated_inside(void) {
    // 45-deg rotation: a 10x10 square rotated 45 deg has diagonal reach ~7.07
    struct Square s = { vec2(0, 0), PI / 4.0f, 10.0f, 10.0f };
    CU_ASSERT_TRUE(square_vs_point(s, vec2(0, 1)));
}

static void test_square_vs_point_rotated_outside(void) {
    struct Square s = { vec2(0, 0), PI / 4.0f, 4.0f, 4.0f };
    // corner reach is ~2.83, point at (3,0) should be outside
    CU_ASSERT_FALSE(square_vs_point(s, vec2(3, 0)));
}

// ─── square_get_world_corners ────────────────────────────────────

static void test_square_get_world_corners_no_rotation(void) {
    struct Square s = { vec2(10, 20), 0.0f, 6.0f, 4.0f };
    Vec2 corners[4];
    square_get_world_corners(s, corners);

    // TL, TR, BR, BL relative to origin (10,20)
    CU_ASSERT_DOUBLE_EQUAL(corners[0].x,  7.0f, 0.01f); // 10 - 3
    CU_ASSERT_DOUBLE_EQUAL(corners[0].y, 18.0f, 0.01f); // 20 - 2
    CU_ASSERT_DOUBLE_EQUAL(corners[1].x, 13.0f, 0.01f); // 10 + 3
    CU_ASSERT_DOUBLE_EQUAL(corners[1].y, 18.0f, 0.01f);
    CU_ASSERT_DOUBLE_EQUAL(corners[2].x, 13.0f, 0.01f);
    CU_ASSERT_DOUBLE_EQUAL(corners[2].y, 22.0f, 0.01f); // 20 + 2
    CU_ASSERT_DOUBLE_EQUAL(corners[3].x,  7.0f, 0.01f);
    CU_ASSERT_DOUBLE_EQUAL(corners[3].y, 22.0f, 0.01f);
}

static void test_square_get_world_corners_90deg(void) {
    struct Square s = { vec2(0, 0), PI / 2.0f, 4.0f, 2.0f };
    Vec2 corners[4];
    square_get_world_corners(s, corners);

    // 90-deg CCW: (x,y) -> (-y, x)
    // local TL=(-2,-1) -> world (1,-2)
    CU_ASSERT_DOUBLE_EQUAL(corners[0].x,  1.0f, 0.01f);
    CU_ASSERT_DOUBLE_EQUAL(corners[0].y, -2.0f, 0.01f);
    // local TR=(2,-1) -> world (1, 2)
    CU_ASSERT_DOUBLE_EQUAL(corners[1].x,  1.0f, 0.01f);
    CU_ASSERT_DOUBLE_EQUAL(corners[1].y,  2.0f, 0.01f);
}

static void test_square_get_world_corners_with_offset(void) {
    struct Square s = { vec2(5, 5), 0.0f, 2.0f, 2.0f };
    Vec2 corners[4];
    square_get_world_corners(s, corners);

    CU_ASSERT_DOUBLE_EQUAL(corners[0].x, 4.0f, 0.01f);
    CU_ASSERT_DOUBLE_EQUAL(corners[0].y, 4.0f, 0.01f);
    CU_ASSERT_DOUBLE_EQUAL(corners[2].x, 6.0f, 0.01f);
    CU_ASSERT_DOUBLE_EQUAL(corners[2].y, 6.0f, 0.01f);
}

// ─── sat_axis_overlaps ───────────────────────────────────────────

static void test_sat_axis_overlaps_true(void) {
    // two unit squares side by side, overlapping on x-axis
    Vec2 a[4] = { vec2(0,0), vec2(2,0), vec2(2,2), vec2(0,2) };
    Vec2 b[4] = { vec2(1,0), vec2(3,0), vec2(3,2), vec2(1,2) };
    Vec2 axis = vec2(1, 0);
    CU_ASSERT_TRUE(sat_axis_overlaps(a, b, axis));
}

static void test_sat_axis_overlaps_false(void) {
    Vec2 a[4] = { vec2(0,0), vec2(1,0), vec2(1,1), vec2(0,1) };
    Vec2 b[4] = { vec2(5,0), vec2(6,0), vec2(6,1), vec2(5,1) };
    Vec2 axis = vec2(1, 0);
    CU_ASSERT_FALSE(sat_axis_overlaps(a, b, axis));
}

static void test_sat_axis_overlaps_touching(void) {
    Vec2 a[4] = { vec2(0,0), vec2(1,0), vec2(1,1), vec2(0,1) };
    Vec2 b[4] = { vec2(1,0), vec2(2,0), vec2(2,1), vec2(1,1) };
    Vec2 axis = vec2(1, 0);
    CU_ASSERT_TRUE(sat_axis_overlaps(a, b, axis));
}

static void test_sat_axis_overlaps_y_axis(void) {
    Vec2 a[4] = { vec2(0,0), vec2(1,0), vec2(1,1), vec2(0,1) };
    Vec2 b[4] = { vec2(0,5), vec2(1,5), vec2(1,6), vec2(0,6) };
    Vec2 axis = vec2(0, 1);
    CU_ASSERT_FALSE(sat_axis_overlaps(a, b, axis));
}

// ─── square_vs_square ────────────────────────────────────────────

static void test_square_vs_square_overlap(void) {
    struct Square a = { vec2(0, 0), 0.0f, 10.0f, 10.0f };
    struct Square b = { vec2(5, 0), 0.0f, 10.0f, 10.0f };
    CU_ASSERT_TRUE(square_vs_square(a, b));
}

static void test_square_vs_square_apart(void) {
    struct Square a = { vec2(0, 0), 0.0f, 4.0f, 4.0f };
    struct Square b = { vec2(20, 0), 0.0f, 4.0f, 4.0f };
    CU_ASSERT_FALSE(square_vs_square(a, b));
}

static void test_square_vs_square_touching(void) {
    struct Square a = { vec2(0, 0), 0.0f, 4.0f, 4.0f };
    struct Square b = { vec2(4, 0), 0.0f, 4.0f, 4.0f };
    CU_ASSERT_TRUE(square_vs_square(a, b));
}

static void test_square_vs_square_same_rotation(void) {
    // both rotated 45 deg => fast AABB path
    struct Square a = { vec2(0, 0), PI / 4.0f, 4.0f, 4.0f };
    struct Square b = { vec2(2, 0), PI / 4.0f, 4.0f, 4.0f };
    CU_ASSERT_TRUE(square_vs_square(a, b));
}

static void test_square_vs_square_same_rotation_apart(void) {
    struct Square a = { vec2(0, 0), PI / 4.0f, 4.0f, 4.0f };
    struct Square b = { vec2(20, 0), PI / 4.0f, 4.0f, 4.0f };
    CU_ASSERT_FALSE(square_vs_square(a, b));
}

static void test_square_vs_square_different_rotation(void) {
    // one rotated, one not => SAT path
    struct Square a = { vec2(0, 0), 0.0f, 4.0f, 4.0f };
    struct Square b = { vec2(2, 0), PI / 4.0f, 4.0f, 4.0f };
    CU_ASSERT_TRUE(square_vs_square(a, b));
}

static void test_square_vs_square_different_rotation_apart(void) {
    struct Square a = { vec2(0, 0), 0.0f, 2.0f, 2.0f };
    struct Square b = { vec2(10, 10), PI / 6.0f, 2.0f, 2.0f };
    CU_ASSERT_FALSE(square_vs_square(a, b));
}

static void test_square_vs_square_contained(void) {
    struct Square a = { vec2(0, 0), 0.0f, 20.0f, 20.0f };
    struct Square b = { vec2(0, 0), PI / 6.0f, 2.0f, 2.0f };
    CU_ASSERT_TRUE(square_vs_square(a, b));
}

// ─── orientation ─────────────────────────────────────────────────

static void test_orientation_positive(void) {
    // b is to the right of a, c is above => positive cross product
    float o = orientation(vec2(0,0), vec2(1,0), vec2(0,1));
    // (b-a)=(1,0), (c-a)=(0,1), cross = 1*1 - 0*0 = 1
    CU_ASSERT_DOUBLE_EQUAL(o, 1.0f, 0.001f);
}

static void test_orientation_collinear(void) {
    // all three points on x-axis => collinear, cross product = 0
    float o = orientation(vec2(0,0), vec2(1,0), vec2(2,0));
    // (b-a)=(1,0), (c-a)=(2,0), cross = 1*0 - 0*2 = 0
    CU_ASSERT_DOUBLE_EQUAL(o, 0.0f, 0.001f);
}

static void test_orientation_negative(void) {
    // (b-a)=(1,0), (c-a)=(-1,0), cross = 1*0 - 0*(-1) = 0 (collinear, opposite direction)
    // Use a point below the line for a truly negative result
    float o = orientation(vec2(0,0), vec2(1,0), vec2(0,-1));
    // (b-a)=(1,0), (c-a)=(0,-1), cross = 1*(-1) - 0*0 = -1
    CU_ASSERT_DOUBLE_EQUAL(o, -1.0f, 0.001f);
}

// ─── line_vs_line ────────────────────────────────────────────────

static void test_line_vs_line_crossing(void) {
    struct Line a = { vec2(0, 0), vec2(10, 10) };
    struct Line b = { vec2(0, 10), vec2(10, 0) };
    CU_ASSERT_TRUE(line_vs_line(a, b));
}

static void test_line_vs_line_parallel(void) {
    struct Line a = { vec2(0, 0), vec2(10, 0) };
    struct Line b = { vec2(0, 5), vec2(10, 5) };
    CU_ASSERT_FALSE(line_vs_line(a, b));
}

static void test_line_vs_line_no_cross(void) {
    struct Line a = { vec2(0, 0), vec2(1, 0) };
    struct Line b = { vec2(5, 5), vec2(6, 5) };
    CU_ASSERT_FALSE(line_vs_line(a, b));
}

static void test_line_vs_line_t_shape(void) {
    // vertical line crosses horizontal
    struct Line a = { vec2(5, 0), vec2(5, 10) };
    struct Line b = { vec2(0, 5), vec2(10, 5) };
    CU_ASSERT_TRUE(line_vs_line(a, b));
}

static void test_line_vs_line_short_segments_miss(void) {
    // lines would cross if extended, but segments don't reach
    struct Line a = { vec2(0, 0), vec2(1, 1) };
    struct Line b = { vec2(5, 0), vec2(6, 1) };
    CU_ASSERT_FALSE(line_vs_line(a, b));
}

// ─── line_vs_circle ──────────────────────────────────────────────

static void test_line_vs_circle_through(void) {
    struct Line l = { vec2(-10, 0), vec2(10, 0) };
    struct Circle c = { vec2(0, 0), 5.0f };
    CU_ASSERT_TRUE(line_vs_circle(l, c));
}

static void test_line_vs_circle_miss(void) {
    struct Line l = { vec2(-10, 10), vec2(10, 10) };
    struct Circle c = { vec2(0, 0), 5.0f };
    CU_ASSERT_FALSE(line_vs_circle(l, c));
}

static void test_line_vs_circle_tangent(void) {
    struct Line l = { vec2(-10, 5), vec2(10, 5) };
    struct Circle c = { vec2(0, 0), 5.0f };
    CU_ASSERT_TRUE(line_vs_circle(l, c));
}

static void test_line_vs_circle_endpoint_inside(void) {
    struct Line l = { vec2(0, 0), vec2(1, 0) };
    struct Circle c = { vec2(0, 0), 5.0f };
    CU_ASSERT_TRUE(line_vs_circle(l, c));
}

static void test_line_vs_circle_degenerate(void) {
    // zero-length line (point) inside circle
    struct Line l = { vec2(0, 0), vec2(0, 0) };
    struct Circle c = { vec2(0, 0), 5.0f };
    CU_ASSERT_TRUE(line_vs_circle(l, c));
}

static void test_line_vs_circle_degenerate_outside(void) {
    struct Line l = { vec2(20, 20), vec2(20, 20) };
    struct Circle c = { vec2(0, 0), 5.0f };
    CU_ASSERT_FALSE(line_vs_circle(l, c));
}

static void test_line_vs_circle_closest_point_on_segment(void) {
    // line goes past circle but closest point on segment is the endpoint
    struct Line l = { vec2(10, 0), vec2(20, 0) };
    struct Circle c = { vec2(0, 0), 5.0f };
    CU_ASSERT_FALSE(line_vs_circle(l, c));
}

// ─── line_vs_square ──────────────────────────────────────────────

static void test_line_vs_square_through(void) {
    struct Line l = { vec2(-10, 0), vec2(10, 0) };
    struct Square s = { vec2(0, 0), 0.0f, 4.0f, 4.0f };
    CU_ASSERT_TRUE(line_vs_square(l, s));
}

static void test_line_vs_square_miss(void) {
    struct Line l = { vec2(-10, 20), vec2(10, 20) };
    struct Square s = { vec2(0, 0), 0.0f, 4.0f, 4.0f };
    CU_ASSERT_FALSE(line_vs_square(l, s));
}

static void test_line_vs_square_endpoint_inside(void) {
    struct Line l = { vec2(0, 0), vec2(100, 100) };
    struct Square s = { vec2(0, 0), 0.0f, 10.0f, 10.0f };
    CU_ASSERT_TRUE(line_vs_square(l, s));
}

static void test_line_vs_square_both_endpoints_inside(void) {
    struct Line l = { vec2(0, 0), vec2(1, 1) };
    struct Square s = { vec2(0, 0), 0.0f, 20.0f, 20.0f };
    CU_ASSERT_TRUE(line_vs_square(l, s));
}

static void test_line_vs_square_edge_cross(void) {
    // line crosses one edge diagonally
    struct Line l = { vec2(-5, 1), vec2(5, 1) };
    struct Square s = { vec2(0, 0), 0.0f, 4.0f, 4.0f };
    CU_ASSERT_TRUE(line_vs_square(l, s));
}

static void test_line_vs_square_rotated(void) {
    struct Line l = { vec2(-10, 0), vec2(10, 0) };
    struct Square s = { vec2(0, 0), PI / 4.0f, 4.0f, 4.0f };
    CU_ASSERT_TRUE(line_vs_square(l, s));
}

static void test_line_vs_square_rotated_miss(void) {
    struct Line l = { vec2(-10, 10), vec2(10, 10) };
    struct Square s = { vec2(0, 0), PI / 4.0f, 4.0f, 4.0f };
    CU_ASSERT_FALSE(line_vs_square(l, s));
}

// ─── main ────────────────────────────────────────────────────────

int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    // circle_vs_circle
    CU_pSuite s1 = CU_add_suite("circle_vs_circle", NULL, NULL);
    CU_add_test(s1, "overlap",     test_circle_vs_circle_overlap);
    CU_add_test(s1, "touching",    test_circle_vs_circle_touching);
    CU_add_test(s1, "apart",       test_circle_vs_circle_apart);
    CU_add_test(s1, "concentric",  test_circle_vs_circle_concentric);
    CU_add_test(s1, "diagonal",    test_circle_vs_circle_diagonal);

    // circle_vs_point
    CU_pSuite s2 = CU_add_suite("circle_vs_point", NULL, NULL);
    CU_add_test(s2, "inside",  test_circle_vs_point_inside);
    CU_add_test(s2, "on_edge", test_circle_vs_point_on_edge);
    CU_add_test(s2, "outside", test_circle_vs_point_outside);
    CU_add_test(s2, "center",  test_circle_vs_point_center);

    // circle_vs_square
    CU_pSuite s3 = CU_add_suite("circle_vs_square", NULL, NULL);
    CU_add_test(s3, "overlap",        test_circle_vs_square_overlap);
    CU_add_test(s3, "apart",          test_circle_vs_square_apart);
    CU_add_test(s3, "circle_inside",  test_circle_vs_square_circle_inside);
    CU_add_test(s3, "touching_edge",  test_circle_vs_square_touching_edge);
    CU_add_test(s3, "rotated",        test_circle_vs_square_rotated);
    CU_add_test(s3, "corner_near_miss", test_circle_vs_square_corner_near_miss);

    // square_vs_point
    CU_pSuite s4 = CU_add_suite("square_vs_point", NULL, NULL);
    CU_add_test(s4, "inside",          test_square_vs_point_inside);
    CU_add_test(s4, "outside",         test_square_vs_point_outside);
    CU_add_test(s4, "on_edge",         test_square_vs_point_on_edge);
    CU_add_test(s4, "on_corner",       test_square_vs_point_on_corner);
    CU_add_test(s4, "rotated_inside",  test_square_vs_point_rotated_inside);
    CU_add_test(s4, "rotated_outside", test_square_vs_point_rotated_outside);

    // square_get_world_corners
    CU_pSuite s5 = CU_add_suite("square_get_world_corners", NULL, NULL);
    CU_add_test(s5, "no_rotation",  test_square_get_world_corners_no_rotation);
    CU_add_test(s5, "90deg",        test_square_get_world_corners_90deg);
    CU_add_test(s5, "with_offset",  test_square_get_world_corners_with_offset);

    // sat_axis_overlaps
    CU_pSuite s6 = CU_add_suite("sat_axis_overlaps", NULL, NULL);
    CU_add_test(s6, "overlap",   test_sat_axis_overlaps_true);
    CU_add_test(s6, "separated", test_sat_axis_overlaps_false);
    CU_add_test(s6, "touching",  test_sat_axis_overlaps_touching);
    CU_add_test(s6, "y_axis",    test_sat_axis_overlaps_y_axis);

    // square_vs_square
    CU_pSuite s7 = CU_add_suite("square_vs_square", NULL, NULL);
    CU_add_test(s7, "overlap",                 test_square_vs_square_overlap);
    CU_add_test(s7, "apart",                   test_square_vs_square_apart);
    CU_add_test(s7, "touching",                test_square_vs_square_touching);
    CU_add_test(s7, "same_rotation",           test_square_vs_square_same_rotation);
    CU_add_test(s7, "same_rotation_apart",     test_square_vs_square_same_rotation_apart);
    CU_add_test(s7, "different_rotation",       test_square_vs_square_different_rotation);
    CU_add_test(s7, "different_rotation_apart", test_square_vs_square_different_rotation_apart);
    CU_add_test(s7, "contained",               test_square_vs_square_contained);

    // orientation
    CU_pSuite s8 = CU_add_suite("orientation", NULL, NULL);
    CU_add_test(s8, "perpendicular", test_orientation_positive);
    CU_add_test(s8, "collinear",     test_orientation_collinear);
    CU_add_test(s8, "opposite",      test_orientation_negative);

    // line_vs_line
    CU_pSuite s9 = CU_add_suite("line_vs_line", NULL, NULL);
    CU_add_test(s9, "crossing",           test_line_vs_line_crossing);
    CU_add_test(s9, "parallel",           test_line_vs_line_parallel);
    CU_add_test(s9, "no_cross",           test_line_vs_line_no_cross);
    CU_add_test(s9, "t_shape",            test_line_vs_line_t_shape);
    CU_add_test(s9, "short_segments_miss", test_line_vs_line_short_segments_miss);

    // line_vs_circle
    CU_pSuite s10 = CU_add_suite("line_vs_circle", NULL, NULL);
    CU_add_test(s10, "through",           test_line_vs_circle_through);
    CU_add_test(s10, "miss",              test_line_vs_circle_miss);
    CU_add_test(s10, "tangent",           test_line_vs_circle_tangent);
    CU_add_test(s10, "endpoint_inside",   test_line_vs_circle_endpoint_inside);
    CU_add_test(s10, "degenerate",        test_line_vs_circle_degenerate);
    CU_add_test(s10, "degenerate_outside", test_line_vs_circle_degenerate_outside);
    CU_add_test(s10, "closest_on_segment", test_line_vs_circle_closest_point_on_segment);

    // line_vs_square
    CU_pSuite s11 = CU_add_suite("line_vs_square", NULL, NULL);
    CU_add_test(s11, "through",              test_line_vs_square_through);
    CU_add_test(s11, "miss",                 test_line_vs_square_miss);
    CU_add_test(s11, "endpoint_inside",      test_line_vs_square_endpoint_inside);
    CU_add_test(s11, "both_endpoints_inside", test_line_vs_square_both_endpoints_inside);
    CU_add_test(s11, "edge_cross",           test_line_vs_square_edge_cross);
    CU_add_test(s11, "rotated",              test_line_vs_square_rotated);
    CU_add_test(s11, "rotated_miss",         test_line_vs_square_rotated_miss);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    unsigned int failures = CU_get_number_of_failures();
    CU_cleanup_registry();

    return failures ? 1 : 0;
}
