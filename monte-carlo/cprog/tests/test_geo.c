/*
 * Copyright (c) 2024 Andreas H. Wolf
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include <math.h>

#include "sphere/sphere.h"
#include "vector/vector.h"
#include "geometry/geometry.h"

static void test_rotate_vec(void **state)
{
   /* Test 1
      Rotate x-axis 90 degrees around z-axis => y-axis 
    */
   vec3   v_1 = { 1.0, 0.0, 0.0 };
   vec3   r_1 = { 0.0, 0.0, 1.0 };
   double phi_1 = pi/2.0;

   /* Test 2
      Rotate z-axis 120 degrees around diagonal axis => x-axis 
    */
   vec3   v_2 = { 0.0, 0.0, 1.0 };
   vec3   r_2 = { 1/sqrt(3.0), 1.0/sqrt(3.0), 1.0/sqrt(3.0) };
   double phi_2 = (2.0*pi)/3.0;

   /* Test 3
      Rotate z-axis 60 degrees around z-axis => z-axis 
    */
   vec3   v_3 = { 0.0, 0.0, 1.0 };
   vec3   r_3 = { 0.0, 0.0, 1.0 };
   double phi_3 = pi/3.0;

   /* Test 4
      Rotate y-axis 0 degrees around z-axis => y-axis 
    */
   vec3   v_4 = { 0.0, 1.0, 0.0 };
   vec3   r_4 = { 0.0, 0.0, 1.0 };
   double phi_4 = 0.0;

   /* Test 5
      Rotate diagonal 180 degrees around z-axis => diagonal with x/y inverted
    */
   vec3   v_5 = { 1.0, 1.0, 1.0 };
   vec3   r_5 = { 0.0, 0.0, 1.0 };
   double phi_5 = pi;

   vec3 out;

   /* Run test 1 */
   rotate_vec(out, v_1, r_1, phi_1);
   assert_true(fabs(out[x_c] - 0.0) < 1E-10);
   assert_true(fabs(out[y_c] - 1.0) < 1E-10);
   assert_true(fabs(out[z_c] - 0.0) < 1E-10);
   
   /* Run test 2 */
   rotate_vec(out, v_2, r_2, phi_2);
   assert_true(fabs(out[x_c] - 1.0) < 1E-10);
   assert_true(fabs(out[y_c] - 0.0) < 1E-10);
   assert_true(fabs(out[z_c] - 0.0) < 1E-10);

   /* Run test 3 */
   rotate_vec(out, v_3, r_3, phi_3);
   assert_true(fabs(out[x_c] - 0.0) < 1E-10);
   assert_true(fabs(out[y_c] - 0.0) < 1E-10);
   assert_true(fabs(out[z_c] - 1.0) < 1E-10);

   /* Run test 4 */
   rotate_vec(out, v_4, r_4, phi_4);
   assert_true(fabs(out[x_c] - 0.0) < 1E-10);
   assert_true(fabs(out[y_c] - 1.0) < 1E-10);
   assert_true(fabs(out[z_c] - 0.0) < 1E-10);

   /* Run test 5 */
   rotate_vec(out, v_5, r_5, phi_5);
   assert_true(fabs(out[x_c] - (-1.0)) < 1E-10);
   assert_true(fabs(out[y_c] - (-1.0)) < 1E-10);
   assert_true(fabs(out[z_c] - 1.0) < 1E-10);
}

static void test_intersect_plane(void **state)
{
   int    retVal;
   double trans;
   vec3   cross;

   /* Test 1
      Particle: Origin on z-axis above x-y plane, direction 'down' along z-axis 
      Plan:     Origin at 0,0,0, Normal 'up' along z-axis 

      Expected cross point: At (0.0,0.0,0.0) and translation value 2.0 (to x-y plane)
   */
   const g_line t1_p = { { 0.0, 0.0, 2.0 }, { 0.0, 0.0, -1.0 } };
   const g_plane t1_r = { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 } };

   const vec3 t1_p_o = { 0.0, 0.0, 2.0 };
   const vec3 t1_p_d = { 0.0, 0.0, -1.0 };
   const vec3 t1_r_o = { 0.0, 0.0, 0.0 };
   const vec3 t1_r_n = { 0.0, 0.0, 1.0 };

   /* Test 2
      Particle: Origin on z-axis below x-y plane, direction 'down' along z-axis 
      Plan:     Origin at 0,0,0, Normal 'up' along z-axis 

      Expected cross point: At (0.0,0.0,0.0) and translation value -2.25 (to x-y plane)
   */
   const g_line t2_p = { { 0.0, 0.0, -2.25 }, { 0.0, 0.0, -1.0 } };
   const g_plane t2_r = { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 } };

   const vec3 t2_p_o = { 0.0, 0.0, -2.25 };
   const vec3 t2_p_d = { 0.0, 0.0, -1.0 };
   const vec3 t2_r_o = { 0.0, 0.0, 0.0 };
   const vec3 t2_r_n = { 0.0, 0.0, 1.0 };

   /* Test 3
      Particle: Origin away from z-axis (-4.0,3.0,-2.25) below x-y plane, direction 'up' along z-axis 
      Plan:     Origin at (1.0,1.0,0.0) and Normal 'up' parallel with z-axis 

      Expected cross point: At (-4.0,3.0,0.0) and translation value 2.25 (to x-y plane)
   */
   const g_line t3_p = { { -4.0, 3.0, -2.25 }, { 0.0, 0.0, 1.0 } };
   const g_plane t3_r = { { 1.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 } };

   const vec3 t3_p_o = { -4.0, 3.0, -2.25 };
   const vec3 t3_p_d = { 0.0, 0.0, 1.0 };
   const vec3 t3_r_o = { 1.0, 1.0, 0.0 };
   const vec3 t3_r_n = { 0.0, 0.0, 1.0 };

   /* Test 4
      Particle: Origin away from z-axis (-4.0,3.0,-2.25), direction 'up' along z-axis 
      Plan:     Origin at (1.0,1.0,0.0) and Normal (with 90 angle to particle) parallel with x-axis 

      Expected cross point: No cross, expect error (-1) indication no common points.
   */
   const g_line t4_p = { { -4.0, 3.0, -2.25 }, { 0.0, 0.0, 1.0 } };
   const g_plane t4_r = { { 1.0, 1.0, 0.0 }, { 1.0, 0.0, 0.0 } };

   const vec3 t4_p_o = { -4.0, 3.0, -2.25 };
   const vec3 t4_p_d = { 0.0, 0.0, 1.0 };
   const vec3 t4_r_o = { 1.0, 1.0, 0.0 };
   const vec3 t4_r_n = { 1.0, 0.0, 0.0 };

   /* Test 5
      Particle: Origin at (0.0,0.0,0.0), direction 'down' along x-axis
      Plan:     Origin at (0.0,0.0,-10.0) and Normal (1.0/sqrt(3),1.0/sqrt(3),1.0/sqrt(3))

      Expected cross point: At (-10.0,0.0,0.0) and translation value 10.0.
   */
   const g_line t5_p = { { 0.0, 0.0, 0.0 }, { -1.0, 0.0, 0.0 } };
   const g_plane t5_r = { { 0.0, 0.0, -10.0 }, { 1.0/sqrt(3.0), 1.0/sqrt(3.0), 1.0/sqrt(3.0) } };

   const vec3 t5_p_o = { 0.0, 0.0, 0.0 };
   const vec3 t5_p_d = { -1.0, 0.0, 0.0 };
   const vec3 t5_r_o = { 0.0, 0.0, -10.0 };
   const vec3 t5_r_n = { 1.0/sqrt(3.0), 1.0/sqrt(3.0), 1.0/sqrt(3.0) };
   
   /* Test 6
      Particle: Origin away from z-axis (1.0,-1.0,0.0), direction 'up' along z-axis 
      Plan:     Origin at (1.0,1.0,0.0) and Normal (with 90 angle to particle) parallel with x-axis 

      Expected cross point: No cross, expect error (-2) indication line is subset of plane
   */
   const g_line t6_p = { { 1.0, -1.0, 0.0 }, { 0.0, 0.0, 1.0 } };
   const g_plane t6_r = { { 1.0, 1.0, 0.0 }, { 1.0, 0.0, 0.0 } };

   const vec3 t6_p_o = { 1.0, -1.0, 0.0 };
   const vec3 t6_p_d = { 0.0, 0.0, 1.0 };
   const vec3 t6_r_o = { 1.0, 1.0, 0.0 };
   const vec3 t6_r_n = { 1.0, 0.0, 0.0 };

   /* Run test 1 */
   retVal = intersect_plane(t1_p, t1_r,
                            &trans, cross);
   assert_int_equal(retVal, 0);

   assert_true(fabs(trans - 2.0) < 1E-10);
   assert_true(fabs(cross[x_c] - 0.0) < 1E-10);
   assert_true(fabs(cross[y_c] - 0.0) < 1E-10);
   assert_true(fabs(cross[z_c] - 0.0) < 1E-10);

   /* Run test 2 */
   retVal = intersect_plane(t2_p, t2_r,
                            &trans, cross);
   assert_int_equal(retVal, 0);
   assert_true(fabs(trans - (-2.25)) < 1E-10);
   assert_true(fabs(cross[x_c] - 0.0) < 1E-10);
   assert_true(fabs(cross[y_c] - 0.0) < 1E-10);
   assert_true(fabs(cross[z_c] - 0.0) < 1E-10);

   /* Run test 3 */
   retVal = intersect_plane(t3_p, t3_r,
                            &trans, cross);
   assert_int_equal(retVal, 0);
   assert_true(fabs(trans - 2.25) < 1E-10);
   assert_true(fabs(cross[x_c] - (-4.0)) < 1E-10);
   assert_true(fabs(cross[y_c] - 3.0) < 1E-10);
   assert_true(fabs(cross[z_c] - 0.0) < 1E-10);

   /* Run test 4 */
   retVal = intersect_plane(t4_p, t4_r,
                            &trans, cross);
   assert_int_equal(retVal, -1);

   /* Run test 5 */
   retVal = intersect_plane(t5_p, t5_r,
                            &trans, cross);
   assert_int_equal(retVal, 0);
   assert_true(fabs(trans - 10.0) < 1E-10);
   assert_true(fabs(cross[x_c] - (-10.0)) < 1E-10);
   assert_true(fabs(cross[y_c] - 0.0) < 1E-10);
   assert_true(fabs(cross[z_c] - 0.0) < 1E-10);

   /* Run test 6 */
   retVal = intersect_plane(t6_p, t6_r,
                            &trans, cross);
   assert_int_equal(retVal, -2);
}

static void test_intersect_rect(void **state)
{
   int    retVal, hit;
   double trans;
   double l1, l2;

   /* Test 1
      Particle:  Origin on z-axis above x-y plane, direction 'down' along z-axis 
      Plane:     Origin at 0,0,0, Normal 'up' along z-axis 
      Rectangle: Edge1 = x-axis / len = 1.0, Edge2 = y-axis / len = 1.0, 

      Expected cross point: At (0.0,0.0,0.0) and translation value 2.0 (to x-y plane)
      Expected hit:         Inside the rectangle -> hit = 1
      Expected local coord: edge1 = 0.0, edge2 = 0.0
   */
   const g_line t1_p = { { 0.0, 0.0, 2.0 }, { 0.0, 0.0, -1.0 } };
   const g_rectangle t1_r = { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 },
                              { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, 1.0, 1.0 };
   
   const vec3 t1_p_o = { 0.0, 0.0, 2.0 };
   const vec3 t1_p_d = { 0.0, 0.0, -1.0 };
   const vec3 t1_r_o = { 0.0, 0.0, 0.0 };
   const vec3 t1_r_n = { 0.0, 0.0, 1.0 };
   const vec3 t1_r_e1 = { 1.0, 0.0, 0.0 };
   const vec3 t1_r_e2 = { 0.0, 1.0, 0.0 };
   const double t1_l_e1 = 1.0;
   const double t1_l_e2 = 1.0;

   /* Test 2
      Particle:  Origin near z-axis above x-y plane, direction 'down' along z-axis 
      Plane:     Origin at 0,0,0, Normal 'up' along z-axis 
      Rectangle: Edge1 = x-axis / len = 1.0, Edge2 = y-axis / len = 1.0, 

      Expected cross point: At (0.5,0.0,0.0) and translation value 2.0 (to x-y plane)
      Expected hit:         Inside the rectangle -> hit = 1
      Expected local coord: edge1 = 0.5, edge2 = 0.0
   */
   const g_line t2_p = { { 0.5, 0.0, 2.0 }, { 0.0, 0.0, -1.0 } };
   const g_rectangle t2_r = { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 },
                              { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, 1.0, 1.0 };

   const vec3 t2_p_o = { 0.5, 0.0, 2.0 };
   const vec3 t2_p_d = { 0.0, 0.0, -1.0 };
   const vec3 t2_r_o = { 0.0, 0.0, 0.0 };
   const vec3 t2_r_n = { 0.0, 0.0, 1.0 };
   const vec3 t2_r_e1 = { 1.0, 0.0, 0.0 };
   const vec3 t2_r_e2 = { 0.0, 1.0, 0.0 };
   const double t2_l_e1 = 1.0;
   const double t2_l_e2 = 1.0;

   /* Test 3
      Particle:  Origin far from z-axis above x-y plane, direction 'down' along z-axis 
      Plane:     Origin at 0,0,0, Normal 'up' along z-axis 
      Rectangle: Edge1 = x-axis / len = 1.0, Edge2 = y-axis / len = 1.0, 

      Expected cross point: At (0.5,4.0,0.0) and translation value 2.0 (to x-y plane)
      Expected hit:         Inside the rectangle -> hit = 0
      Expected local coord: edge1 = 0.5, edge2 = 4.0
   */
   const g_line t3_p = { { 0.5, 4.0, 2.0 }, { 0.0, 0.0, -1.0 } };
   const g_rectangle t3_r = { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 },
                              { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, 1.0, 1.0 };

   const vec3 t3_p_o = { 0.5, 4.0, 2.0 };
   const vec3 t3_p_d = { 0.0, 0.0, -1.0 };
   const vec3 t3_r_o = { 0.0, 0.0, 0.0 };
   const vec3 t3_r_n = { 0.0, 0.0, 1.0 };
   const vec3 t3_r_e1 = { 1.0, 0.0, 0.0 };
   const vec3 t3_r_e2 = { 0.0, 1.0, 0.0 };
   const double t3_l_e1 = 1.0;
   const double t3_l_e2 = 1.0;

   /* Test 4
      Particle:  Origin far from z-axis above x-y plane, direction 'away' from origin 
      Plane:     Origin at 0,0,0, Normal 'up' along z-axis 
      Rectangle: Edge1 = x-axis / len = 1.0, Edge2 = y-axis / len = 1.0, 

      Expected cross point: At (0.0,0.0,0.0) and translation value -sqrt(20.25) = -4.5 (to x-y plane)
      Expected hit:         Inside the rectangle -> hit = 1
      Expected local coord: edge1 = 0.0, edge2 = 0.0
   */
   const g_line t4_p = { { 0.5, 4.0, 2.0 }, { 0.5/4.5, 4.0/4.5, 2.0/4.5 } };
   const g_rectangle t4_r = { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 },
                              { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, 1.0, 1.0 };

   const vec3 t4_p_o = { 0.5, 4.0, 2.0 };
   const vec3 t4_p_d = { 0.5/4.5, 4.0/4.5, 2.0/4.5 };
   const vec3 t4_r_o = { 0.0, 0.0, 0.0 };
   const vec3 t4_r_n = { 0.0, 0.0, 1.0 };
   const vec3 t4_r_e1 = { 1.0, 0.0, 0.0 };
   const vec3 t4_r_e2 = { 0.0, 1.0, 0.0 };
   const double t4_l_e1 = 1.0;
   const double t4_l_e2 = 1.0;

   /* Test 5
      Particle:  Origin on z-axis above x-y plane, direction 'down' along z-axis 
      Plane:     Origin at 0,2.0,0, Normal 'up' along z-axis 
      Rectangle: Edge1 = x-axis / len = 1.0, Edge2 = y-axis / len = 1.0, 

      Expected cross point: At (0.0,0.0,0.0) and translation value 2.0 (to x-y plane)
      Expected hit:         Inside the rectangle -> hit = 0
      Expected local coord: edge1 = 0.0, edge2 = -2.0
   */
   const g_line t5_p = { { 0.0, 0.0, 2.0 }, { 0.0, 0.0, -1.0 } };
   const g_rectangle t5_r = { { 0.0, 2.0, 0.0 }, { 0.0, 0.0, 1.0 },
                              { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, 1.0, 1.0 };

   const vec3 t5_p_o = { 0.0, 0.0, 2.0 };
   const vec3 t5_p_d = { 0.0, 0.0, -1.0 };
   const vec3 t5_r_o = { 0.0, 2.0, 0.0 };
   const vec3 t5_r_n = { 0.0, 0.0, 1.0 };
   const vec3 t5_r_e1 = { 1.0, 0.0, 0.0 };
   const vec3 t5_r_e2 = { 0.0, 1.0, 0.0 };
   const double t5_l_e1 = 1.0;
   const double t5_l_e2 = 1.0;

   /* Test 6
      Particle:  Origin on z-axis above x-y plane, direction 'down' along z-axis 
      Plane:     Origin at 0,1.0,0, Normal 'sideways' along y-axis 
      Rectangle: Edge1 = x-axis / len = 1.0, Edge2 = z-axis / len = 1.0, 

      Expected cross point: None (error -1)
      Expected hit:         None
      Expected local coord: None
   */
   const g_line t6_p = { { 0.0, 0.0, 2.0 }, { 0.0, 0.0, -1.0 } };
   const g_rectangle t6_r = { { 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 },
                              { 1.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 }, 1.0, 1.0 };

   const vec3 t6_p_o = { 0.0, 0.0, 2.0 };
   const vec3 t6_p_d = { 0.0, 0.0, -1.0 };
   const vec3 t6_r_o = { 0.0, 1.0, 0.0 };
   const vec3 t6_r_n = { 0.0, 1.0, 0.0 };
   const vec3 t6_r_e1 = { 1.0, 0.0, 0.0 };
   const vec3 t6_r_e2 = { 0.0, 0.0, 1.0 };
   const double t6_l_e1 = 1.0;
   const double t6_l_e2 = 1.0;

   /* Test 7
      Particle:  Origin on z-axis above x-y plane, direction 'down' along z-axis 
      Plane:     Origin at 0,0,0, Normal 'sideways' along y-axis 
      Rectangle: Edge1 = x-axis / len = 1.0, Edge2 = z-axis / len = 1.0, 

      Expected cross point: None/line subset (error -2)
      Expected hit:         None
      Expected local coord: None
   */
   const g_line t7_p = { { 0.0, 0.0, 2.0 }, { 0.0, 0.0, -1.0 } };
   const g_rectangle t7_r = { { 0.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 },
                              { 1.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 }, 1.0, 1.0 };

   const vec3 t7_p_o = { 0.0, 0.0, 2.0 };
   const vec3 t7_p_d = { 0.0, 0.0, -1.0 };
   const vec3 t7_r_o = { 0.0, 0.0, 0.0 };
   const vec3 t7_r_n = { 0.0, 1.0, 0.0 };
   const vec3 t7_r_e1 = { 1.0, 0.0, 0.0 };
   const vec3 t7_r_e2 = { 0.0, 0.0, 1.0 };
   const double t7_l_e1 = 1.0;
   const double t7_l_e2 = 1.0;
   
   /* Run test 1 */
   retVal = intersect_rect(t1_p, t1_r,
                           &trans, &hit, &l1, &l2);
   assert_int_equal(retVal, 0);
   assert_true(fabs(trans - 2.0) < 1E-10);
   assert_int_equal(hit, 1);
   assert_true(fabs(l1 - 0.0) < 1E-10);
   assert_true(fabs(l2 - 0.0) < 1E-10);

   /* Run test 2 */
   retVal = intersect_rect(t2_p, t2_r,
                           &trans, &hit, &l1, &l2);
   assert_int_equal(retVal, 0);
   assert_true(fabs(trans - 2.0) < 1E-10);
   assert_int_equal(hit, 1);
   assert_true(fabs(l1 - 0.5) < 1E-10);
   assert_true(fabs(l2 - 0.0) < 1E-10);

   /* Run test 3 */
   retVal = intersect_rect(t3_p, t3_r,
                           &trans, &hit, &l1, &l2);
   assert_int_equal(retVal, 0);
   assert_true(fabs(trans - 2.0) < 1E-10);
   assert_int_equal(hit, 0);
   assert_true(fabs(l1 - 0.5) < 1E-10);
   assert_true(fabs(l2 - 4.0) < 1E-10);

   /* Run test 4 */
   retVal = intersect_rect(t4_p, t4_r,
                           &trans, &hit, &l1, &l2);
   assert_int_equal(retVal, 0);
   assert_true(fabs(trans - (-4.5)) < 1E-10);
   assert_int_equal(hit, 1);
   assert_true(fabs(l1 - 0.0) < 1E-10);
   assert_true(fabs(l2 - 0.0) < 1E-10);

   /* Run test 5 */
   retVal = intersect_rect(t5_p, t5_r,
                           &trans, &hit, &l1, &l2);
   assert_int_equal(retVal, 0);
   assert_true(fabs(trans - 2.0) < 1E-10);
   assert_int_equal(hit, 0);
   assert_true(fabs(l1 - 0.0) < 1E-10);
   assert_true(fabs(l2 - (-2.0)) < 1E-10);

   /* Run test 6 */
   retVal = intersect_rect(t6_p, t6_r,
                           &trans, &hit, &l1, &l2);
   assert_int_equal(retVal, -1);

   /* Run test 7 */
   retVal = intersect_rect(t7_p, t7_r,
                           &trans, &hit, &l1, &l2);
   assert_int_equal(retVal, -2);
}

static void test_intersect_box(void **state)
{
   /* Test 1
      Particle:  Origin on z-axis above x-y plane, direction 'down' along z-axis 
      Box:       Origin at 0,0,0
                 Edge1 = x-axis / len = 1.0, Edge2 = y-axis / len = 1.0, Edge3 = z-axis / len = 1.0

      Expected cross points: At (0.0,0.0,0.5) and translation value 1.5 (to x-y plane),
                             At (0.0,0.0,-0.5) and translation value 2.5 (to x-y plane)

   */
   const g_line t1_p = { { 0.0, 0.0, 2.0 }, { 0.0, 0.0, -1.0 } };
   const g_box  t1_b = { { 0.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 },
                         0.5, 0.5, 0.5 };

   const vec3 t1_p_o = { 0.0, 0.0, 2.0 };
   const vec3 t1_p_d = { 0.0, 0.0, -1.0 };
   const vec3 t1_b_o = { 0.0, 0.0, 0.0 };
   const vec3 t1_b_e1 = { 1.0, 0.0, 0.0 };
   const vec3 t1_b_e2 = { 0.0, 1.0, 0.0 };
   const vec3 t1_b_e3 = { 0.0, 0.0, 1.0 };
   const double t1_b_l1    = 0.5;
   const double t1_b_l2    = 0.5;
   const double t1_b_l3    = 0.5;

   /* Test 2
      Particle:  Origin on x-axis below z-y plane, direction 'up' along x-axis 
      Box:       Origin at 0,0,0
                 Edge1 = x-axis / len = 1.0, Edge2 = y-axis / len = 1.0, Edge3 = z-axis / len = 1.0

      Expected cross points: At (-0.5,0.0,0.0) and translation value 2.5 (to x-y plane),
                             At (0.5,0.0,0.0) and translation value 3.5 (to x-y plane)

   */
   const g_line t2_p = { { -3.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 } };
   const g_box  t2_b = { { 0.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 },
                         0.5, 0.5, 0.5 };

   const vec3 t2_p_o = { -3.0, 0.0, 0.0 };
   const vec3 t2_p_d = { 1.0, 0.0, 0.0 };
   const vec3 t2_b_o = { 0.0, 0.0, 0.0 };
   const vec3 t2_b_e1 = { 1.0, 0.0, 0.0 };
   const vec3 t2_b_e2 = { 0.0, 1.0, 0.0 };
   const vec3 t2_b_e3 = { 0.0, 0.0, 1.0 };
   const double t2_b_l1    = 0.5;
   const double t2_b_l2    = 0.5;
   const double t2_b_l3    = 0.5;

   /* Test 3
      Particle:  Origin on 0,0,0, direction 'up' along y-axis 
      Box:       Origin at 0,0,0
                 Edge1 = x-axis / len = 1.0, Edge2 = y-axis / len = 1.0, Edge3 = z-axis / len = 1.0

      Expected cross points: At (0.0,-0.5,0.0) and translation value -0.5 (to x-y plane),
                             At (0.0,0.5,0.0) and translation value 0.5 (to x-y plane)

   */
   const g_line t3_p = { { 0.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 } };
   const g_box  t3_b = { { 0.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 },
                         0.5, 0.5, 0.5 };

   const vec3 t3_p_o = { 0.0, 0.0, 0.0 };
   const vec3 t3_p_d = { 0.0, 1.0, 0.0 };
   const vec3 t3_b_o = { 0.0, 0.0, 0.0 };
   const vec3 t3_b_e1 = { 1.0, 0.0, 0.0 };
   const vec3 t3_b_e2 = { 0.0, 1.0, 0.0 };
   const vec3 t3_b_e3 = { 0.0, 0.0, 1.0 };
   const double t3_b_l1    = 0.5;
   const double t3_b_l2    = 0.5;
   const double t3_b_l3    = 0.5;

   /* Test 4
      Particle:  Origin on 0.5,0,0, direction diagonal towards z-axis
      Box:       Origin at 0,0,0
                 Edge1 = x-axis / len = 1.0, Edge2 = y-axis / len = 1.0, Edge3 = z-axis / len = 1.0

      Expected cross points: At (0.5,0.0,0.0) and translation value 0.0 (to z-y plane),
                             At (0.0,0.0,0.5) and translation value 1/sqrt(2) (to x-y plane)
   */
   const g_line t4_p = { { 0.5, 0.0, 0.0 }, { -1.0/sqrt(2.0), 0.0, 1.0/sqrt(2.0) } };
   const g_box  t4_b = { { 0.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 },
                         0.5, 0.5, 0.5 };

   const vec3 t4_p_o = { 0.5, 0.0, 0.0 };
   const vec3 t4_p_d = { -1.0/sqrt(2.0), 0.0, 1.0/sqrt(2.0) };
   const vec3 t4_b_o = { 0.0, 0.0, 0.0 };
   const vec3 t4_b_e1 = { 1.0, 0.0, 0.0 };
   const vec3 t4_b_e2 = { 0.0, 1.0, 0.0 };
   const vec3 t4_b_e3 = { 0.0, 0.0, 1.0 };
   const double t4_b_l1    = 0.5;
   const double t4_b_l2    = 0.5;
   const double t4_b_l3    = 0.5;

   /* Test 5
      Particle:  Origin on 0.65,0.1,0.25, direction diagonal down towards z-axis
      Box:       Origin at 0,0,0
                 Edge1 = x-axis / len = 1.0, Edge2 = y-axis / len = 1.0, Edge3 = z-axis / len = 1.0

      Expected cross points: At (0.5,0.1,0.1) and translation value 0.15*sqrt(2) (to z-y plane),
                             At (-0.1,0.1,-0.5) and translation value 0.75*sqrt(2) (to x-y plane)

   */
   const g_line t5_p = { { 0.65, 0.1, 0.25 }, { -1.0/sqrt(2.0), 0.0, -1.0/sqrt(2.0) } };
   const g_box  t5_b = { { 0.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 },
                         0.5, 0.5, 0.5 };

   const vec3 t5_p_o = { 0.65, 0.1, 0.25 };
   const vec3 t5_p_d = { -1.0/sqrt(2.0), 0.0, -1.0/sqrt(2.0) };
   const vec3 t5_b_o = { 0.0, 0.0, 0.0 };
   const vec3 t5_b_e1 = { 1.0, 0.0, 0.0 };
   const vec3 t5_b_e2 = { 0.0, 1.0, 0.0 };
   const vec3 t5_b_e3 = { 0.0, 0.0, 1.0 };
   const double t5_b_l1    = 0.5;
   const double t5_b_l2    = 0.5;
   const double t5_b_l3    = 0.5;

   /* Test 6
      Particle:  Origin on -0.65,0.1,0.25, direction diagonal down towards z-axis
      Box:       Origin at 0,0,0
                 Edge1 = x-axis / len = 1.0, Edge2 = y-axis / len = 1.0, Edge3 = z-axis / len = 1.0

      Expected cross points: At (-0.5,0.1,0.1) and translation value 0.15*sqrt(2) (to z-y plane),
                             At (0.1,0.1,-0.5) and translation value 0.75*sqrt(2) (to x-y plane)

   */
   const g_line t6_p = { { -0.65, 0.1, 0.25 }, { 1.0/sqrt(2.0), 0.0, -1.0/sqrt(2.0) } };
   const g_box  t6_b = { { 0.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 },
                         0.5, 0.5, 0.5 };

   const vec3 t6_p_o = { -0.65, 0.1, 0.25 };
   const vec3 t6_p_d = { 1.0/sqrt(2.0), 0.0, -1.0/sqrt(2.0) };
   const vec3 t6_b_o = { 0.0, 0.0, 0.0 };
   const vec3 t6_b_e1 = { 1.0, 0.0, 0.0 };
   const vec3 t6_b_e2 = { 0.0, 1.0, 0.0 };
   const vec3 t6_b_e3 = { 0.0, 0.0, 1.0 };
   const double t6_b_l1    = 0.5;
   const double t6_b_l2    = 0.5;
   const double t6_b_l3    = 0.5;
   
   /* Test 7
      Particle:  Origin on 0.1,0.65,0.25, direction diagonal down towards z-axis
      Box:       Origin at 0,0,0
                 Edge1 = x-axis / len = 1.0, Edge2 = y-axis / len = 1.0, Edge3 = z-axis / len = 1.0

      Expected cross points: At (0.1,0.5,0.1) and translation value 0.15*sqrt(2) (to z-y plane),
                             At (0.1,-0.1,-0.5) and translation value 0.75*sqrt(2) (to x-y plane)

   */
   const g_line t7_p = { { 0.1, 0.65, 0.25 }, { 0.0, -1.0/sqrt(2.0), -1.0/sqrt(2.0) } };
   const g_box  t7_b = { { 0.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 },
                         0.5, 0.5, 0.5 };

   const vec3 t7_p_o = { 0.1, 0.65, 0.25 };
   const vec3 t7_p_d = { 0.0, -1.0/sqrt(2.0), -1.0/sqrt(2.0) };
   const vec3 t7_b_o = { 0.0, 0.0, 0.0 };
   const vec3 t7_b_e1 = { 1.0, 0.0, 0.0 };
   const vec3 t7_b_e2 = { 0.0, 1.0, 0.0 };
   const vec3 t7_b_e3 = { 0.0, 0.0, 1.0 };
   const double t7_b_l1    = 0.5;
   const double t7_b_l2    = 0.5;
   const double t7_b_l3    = 0.5;

   /* Test 8
      Particle:  Origin on 0.1,-0.65,0.25, direction diagonal down towards z-axis
      Box:       Origin at 0,0,0
                 Edge1 = x-axis / len = 1.0, Edge2 = y-axis / len = 1.0, Edge3 = z-axis / len = 1.0

      Expected cross points: At (0.1,-0.5,0.1) and translation value 0.15*sqrt(2) (to z-y plane),
                             At (0.1,0.1,-0.5) and translation value 0.75*sqrt(2) (to x-y plane)
   */
   const g_line t8_p = { { 0.1, -0.65, 0.25 }, { 0.0, 1.0/sqrt(2.0), -1.0/sqrt(2.0) } };
   const g_box  t8_b = { { 0.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 },
                         0.5, 0.5, 0.5 };

   const vec3 t8_p_o = { 0.1, -0.65, 0.25 };
   const vec3 t8_p_d = { 0.0, 1.0/sqrt(2.0), -1.0/sqrt(2.0) };
   const vec3 t8_b_o = { 0.0, 0.0, 0.0 };
   const vec3 t8_b_e1 = { 1.0, 0.0, 0.0 };
   const vec3 t8_b_e2 = { 0.0, 1.0, 0.0 };
   const vec3 t8_b_e3 = { 0.0, 0.0, 1.0 };
   const double t8_b_l1    = 0.5;
   const double t8_b_l2    = 0.5;
   const double t8_b_l3    = 0.5;

   /* Test output */
   int    hit, result;
   double tr[2];
   double l1[2], l2[2], l3[2];
   
   /* Run test 1 */
   result = intersect_box(t1_p, t1_b,
                          tr, &hit,
                          l1, l2, l3);
   assert_int_equal(result, 0);
   assert_int_equal(hit, 2);
   /* Check order */
   assert_true(fabs(tr[0] - 1.5) < 1E-10);
   assert_true(fabs(tr[1] - 2.5) < 1E-10);
   /* Check coordinates */
   assert_true(fabs(l1[0] - 0.0) < 1E-10);
   assert_true(fabs(l2[0] - 0.0) < 1E-10);
   assert_true(fabs(l3[0] - 0.5) < 1E-10);
   assert_true(fabs(l1[1] - 0.0) < 1E-10);
   assert_true(fabs(l2[1] - 0.0) < 1E-10);
   assert_true(fabs(l3[1] - (-0.5)) < 1E-10);

   /* Run test 2 */
   result = intersect_box(t2_p, t2_b,
                          tr, &hit,
                          l1, l2, l3);
   assert_int_equal(result, 0);
   assert_int_equal(hit, 2);
   /* Check order */
   assert_true(fabs(tr[0] - 3.5) < 1E-10);
   assert_true(fabs(tr[1] - 2.5) < 1E-10);
   /* Check coordinates */
   assert_true(fabs(l1[0] - 0.5) < 1E-10);
   assert_true(fabs(l2[0] - 0.0) < 1E-10);
   assert_true(fabs(l3[0] - 0.0) < 1E-10);
   assert_true(fabs(l1[1] - (-0.5)) < 1E-10);
   assert_true(fabs(l2[1] - 0.0) < 1E-10);
   assert_true(fabs(l3[1] - 0.0) < 1E-10);

   /* Run test 3 */
   result = intersect_box(t3_p, t3_b,
                          tr, &hit,
                          l1, l2, l3);
   assert_int_equal(result, 0);
   assert_int_equal(hit, 2);
   /* Check order */
   assert_true(fabs(tr[0] - 0.5) < 1E-10);
   assert_true(fabs(tr[1] - (-0.5)) < 1E-10);
   /* Check coordinates */
   assert_true(fabs(l1[0] - 0.0) < 1E-10);
   assert_true(fabs(l2[0] - 0.5) < 1E-10);
   assert_true(fabs(l3[0] - 0.0) < 1E-10);
   assert_true(fabs(l1[1] - 0.0) < 1E-10);
   assert_true(fabs(l2[1] - (-0.5)) < 1E-10);
   assert_true(fabs(l3[1] - 0.0) < 1E-10);

   /* Run test 4 */
   result = intersect_box(t4_p, t4_b,
                          tr, &hit,
                          l1, l2, l3);
   assert_int_equal(result, 0);
   assert_int_equal(hit, 2);
   /* Check order */
   assert_true(fabs(tr[0] - 0.0) < 1E-10);
   assert_true(fabs(tr[1] - (1.0/sqrt(2.0))) < 1E-10);
   /* Check coordinates */
   assert_true(fabs(l1[0] - 0.5) < 1E-10);
   assert_true(fabs(l2[0] - 0.0) < 1E-10);
   assert_true(fabs(l3[0] - 0.0) < 1E-10);
   assert_true(fabs(l1[1] - 0.0) < 1E-10);
   assert_true(fabs(l2[1] - 0.0) < 1E-10);
   assert_true(fabs(l3[1] - 0.5) < 1E-10);

   /* Run test 5 */
   result = intersect_box(t5_p, t5_b,
                          tr, &hit,
                          l1, l2, l3);
   assert_int_equal(result, 0);
   assert_int_equal(hit, 2);
   /* Check order */
   assert_true(fabs(tr[0] - 0.15*sqrt(2)) < 1E-10);
   assert_true(fabs(tr[1] - 0.75*sqrt(2)) < 1E-10);   
   /* Check coordinates */
   assert_true(fabs(l1[0] - 0.5) < 1E-10);
   assert_true(fabs(l2[0] - 0.1) < 1E-10);
   assert_true(fabs(l3[0] - 0.1) < 1E-10);
   assert_true(fabs(l1[1] - (-0.1)) < 1E-10);
   assert_true(fabs(l2[1] - 0.1) < 1E-10);
   assert_true(fabs(l3[1] - (-0.5)) < 1E-10);

   /* Run test 6 */
   result = intersect_box(t6_p, t6_b,
                          tr, &hit,
                          l1, l2, l3);
   assert_int_equal(result, 0);
   assert_int_equal(hit, 2);
   /* Check order */
   assert_true(fabs(tr[0] - 0.15*sqrt(2)) < 1E-10);
   assert_true(fabs(tr[1] - 0.75*sqrt(2)) < 1E-10);   
   /* Check coordinates */
   assert_true(fabs(l1[0] - (-0.5)) < 1E-10);
   assert_true(fabs(l2[0] - 0.1) < 1E-10);
   assert_true(fabs(l3[0] - 0.1) < 1E-10);
   assert_true(fabs(l1[1] - 0.1) < 1E-10);
   assert_true(fabs(l2[1] - 0.1) < 1E-10);
   assert_true(fabs(l3[1] - (-0.5)) < 1E-10);

   /* Run test 7 */
   result = intersect_box(t7_p, t7_b,
                          tr, &hit,
                          l1, l2, l3);
   assert_int_equal(result, 0);
   assert_int_equal(hit, 2);
   /* Check order */
   assert_true(fabs(tr[0] - 0.15*sqrt(2)) < 1E-10);
   assert_true(fabs(tr[1] - 0.75*sqrt(2)) < 1E-10);   
   /* Check coordinates */
   assert_true(fabs(l1[0] - 0.1) < 1E-10);
   assert_true(fabs(l2[0] - 0.5) < 1E-10);
   assert_true(fabs(l3[0] - 0.1) < 1E-10);
   assert_true(fabs(l1[1] - 0.1) < 1E-10);
   assert_true(fabs(l2[1] - (-0.1)) < 1E-10);
   assert_true(fabs(l3[1] - (-0.5)) < 1E-10);

   /* Run test 8 */
   result = intersect_box(t8_p, t8_b,
                          tr, &hit,
                          l1, l2, l3);
   assert_int_equal(result, 0);
   assert_int_equal(hit, 2);
   /* Check order */
   assert_true(fabs(tr[0] - 0.15*sqrt(2)) < 1E-10);
   assert_true(fabs(tr[1] - 0.75*sqrt(2)) < 1E-10);   
   /* Check coordinates */
   assert_true(fabs(l1[0] - 0.1) < 1E-10);
   assert_true(fabs(l2[0] - (-0.5)) < 1E-10);
   assert_true(fabs(l3[0] - 0.1) < 1E-10);
   assert_true(fabs(l1[1] - 0.1) < 1E-10);
   assert_true(fabs(l2[1] - 0.1) < 1E-10);
   assert_true(fabs(l3[1] - (-0.5)) < 1E-10);
}

int main(int argc, char**argv)
{
   const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_rotate_vec),
      cmocka_unit_test(test_intersect_plane),
      cmocka_unit_test(test_intersect_rect),
      cmocka_unit_test(test_intersect_box),
   };

   return cmocka_run_group_tests(tests, NULL, NULL);
}
