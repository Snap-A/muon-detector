#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include <math.h>

#include "vector/vector.h"

static void test_scale(void **state)
{
   const vec3 in0 = { 1.0, 0.5, -2.0 };
   vec3 scr = { 0.0, 0.0, 0.0 };
   
   scale_vec2(scr, in0, 10.0);

   assert_true(fabs(scr[0] - 10.0) < 1E-10);
   assert_true(fabs(scr[1] - 5.0) < 1E-10);
   assert_true(fabs(scr[2] - (-20.0)) < 1E-10);

   scale_vec(scr, -0.5);
   
   assert_true(fabs(scr[0] - -(5.0)) < 1E-10);
   assert_true(fabs(scr[1] - (-2.5)) < 1E-10);
   assert_true(fabs(scr[2] - 10.0) < 1E-10);
}

static void test_diff(void **state)
{
   const vec3 in0 = { 1.0, 0.5, -2.0 };
   const vec3 in1 = { -1.0, 0.5, 10.0 };
   vec3 scr = { 0.0, 0.0, 0.0 };

   diff_vec(scr, in0, in1);
   assert_true(fabs(scr[0] - 2.0) < 1E-10);
   assert_true(fabs(scr[1] - 0.0) < 1E-10);
   assert_true(fabs(scr[2] - (-12.0)) < 1E-10);   

   diff_vec(scr, in1, in0);
   assert_true(fabs(scr[0] - (-2.0)) < 1E-10);
   assert_true(fabs(scr[1] - 0.0) < 1E-10);
   assert_true(fabs(scr[2] - 12.0) < 1E-10);   
}

static void test_add(void **state)
{
   const vec3 in0 = { 1.0, 0.5, -2.0 };
   const vec3 in1 = { -1.0, 0.5, 10.0 };
   vec3 scr = { 0.0, 0.0, 0.0 };

   add_vec(scr, in0, in1);
   assert_true(fabs(scr[0] - 0.0) < 1E-10);
   assert_true(fabs(scr[1] - 1.0) < 1E-10);
   assert_true(fabs(scr[2] - 8.0) < 1E-10);   
}

static void test_dot(void **state)
{
   const vec3 in0 = { 1.0, 0.5, -2.0 };
   const vec3 in1 = { -1.0, 0.5, 10.0 };
   double dot;

   dot = dot_vec(in0, in1);
   assert_true(fabs(dot - (-1.0 + 0.25 - 20.0)) < 1E-10);      
}

static void test_cross(void **state)
{
   /* Test 1: x cross y => z */
   const vec3 in0_1 = { 1.0, 0.0, 0.0 };
   const vec3 in1_1 = { 0.0, 1.0, 0.0 };

   /* Test 2: x cross z => -y */
   const vec3 in0_2 = { 1.0, 0.0, 0.0 };
   const vec3 in1_2 = { 0.0, 0.0, 1.0 };

   vec3 out;

   cross_vec(out, in0_1, in1_1);
   assert_true(fabs(out[0] - 0.0) < 1E-10);
   assert_true(fabs(out[1] - 0.0) < 1E-10);
   assert_true(fabs(out[2] - 1.0) < 1E-10);

   cross_vec(out, in0_2, in1_2);
   assert_true(fabs(out[0] - 0.0) < 1E-10);
   assert_true(fabs(out[1] - (-1.0)) < 1E-10);
   assert_true(fabs(out[2] - 0.0) < 1E-10);
}

static void test_mul_matrix(void **state)
{
   /* Test 1 */
   const vec3 in_1 = { 1.0, -1.0, 2.0 };
   const mat33 M_1 = { 1.0, 1.0, 1.0,
                       1.0, 1.0, 1.0,
                       1.0, 1.0, 1.0 };

   /* Test 2 */
   const vec3 in_2 = { 1.0, -1.0, 2.0 };
   const mat33 M_2 = { 1.0, 0.0, 0.0,
                       0.0, 1.0, 0.0,
                       0.0, 0.0, 1.0 };

   /* Test 3 */
   const vec3 in_3 = { 1.0, -1.0, 2.0 };
   const mat33 M_3 = { 0.0, 1.0, 0.0,
                       -2.0, 0.0, 0.0,
                       0.0, 0.0, 0.5 };

   vec3 out;

   /* Run test 1 */
   mul_matrix(out, M_1, in_1);
   assert_true(fabs(out[0] - 2.0) < 1E-10);
   assert_true(fabs(out[1] - 2.0) < 1E-10);
   assert_true(fabs(out[2] - 2.0) < 1E-10);

   /* Run test 2 */
   mul_matrix(out, M_2, in_2);
   assert_true(fabs(out[0] - 1.0) < 1E-10);
   assert_true(fabs(out[1] - (-1.0)) < 1E-10);
   assert_true(fabs(out[2] - 2.0) < 1E-10);

   /* Run test 3 */
   mul_matrix(out, M_3, in_3);
   assert_true(fabs(out[0] - (-1.0)) < 1E-10);
   assert_true(fabs(out[1] - (-2.0)) < 1E-10);
   assert_true(fabs(out[2] - 1.0) < 1E-10);
}

int main(int argc, char**argv)
{
   const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_scale),
      cmocka_unit_test(test_diff),
      cmocka_unit_test(test_add),
      cmocka_unit_test(test_dot),
      cmocka_unit_test(test_cross),
      cmocka_unit_test(test_mul_matrix),
   };

   return cmocka_run_group_tests(tests, NULL, NULL);
}
