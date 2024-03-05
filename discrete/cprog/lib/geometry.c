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

#include "geometry/geometry.h"
#include "vector/vector.h"
#include <math.h>
#include <stdio.h>

void rotate_vec(vec3 v_out, const vec3 v_in,
                const vec3 rot_axis, double phi)
{
   double r_proj;
   double len_v_ort;
   vec3   v_par;
   vec3   v_ort;
   vec3   v_local1;
   vec3   v_local2;
   vec3   v_ort_prime;

   /* Decompose v_in into parts that are parallel to the rotation axis (will not change)
      and parts that are orthogonal (will be rotated) */
   r_proj = dot_vec(v_in, rot_axis);

   scale_vec2(v_par, rot_axis, r_proj);
   diff_vec(v_ort, v_in, v_par);

   len_v_ort = sqrt(dot_vec(v_ort, v_ort));
   if (fabs(len_v_ort) < 1.0E-10)
   {
      /* v_in is parallel to rotational axis, return unchanged v_in */
      scale_vec2(v_out, v_in, 1.0);
      return;
   }

   /* Create local coordinate vectors in plane to rotate */
   /* local_1 = Normalized v_ort */
   scale_vec2(v_local1, v_ort, 1.0/len_v_ort);

   /* local_2 = cross of rotation axis and local_1 */
   cross_vec(v_local2, rot_axis, v_local1);
   
   /* Rotate local coordinates with the angle phi */
   scale_vec(v_local1, cos(phi));
   scale_vec(v_local2, sin(phi));

   /* Recreate rotated v_ort => v_ort' */
   add_vec(v_ort_prime, v_local1, v_local2);
   scale_vec(v_ort_prime, len_v_ort);
   
   /* Add back invariant vector part */
   add_vec(v_out, v_ort_prime, v_par);
}

int intersect_plane(const g_line line,
                    const g_plane plane,
                    double *translation, vec3 cross)
{
   double N_proj;
   double D_proj;
   vec3   Delta;
   vec3   Move;

   /* Displacement between origins (from line to plane)*/
   diff_vec(Delta, plane.origin, line.origin);

   /* Project displacement onto normal of plane -> Separation of origin from plane */
   D_proj = dot_vec(Delta, plane.normal);

   /* Project direction of line onto normal of plane */
   N_proj = dot_vec(line.direction, plane.normal);

   if (fabs(N_proj) < 1.0E-10)
   {
      /* Too small of a projection value: Direction is parallel to plane */
      if (fabs(D_proj) < 1.0E-10)
      {
         /* Too small of a distance: The points on the line are a subset of the plane */
         return -2;
      }
      else
      {
         /* The points on the line are a distinct set from the plane and never 'crosses' */
         return -1;
      }
   }

   /* Ratio of N_proj and D_proj is the length of particle direction vector that
      moves from the particles origin to the (infinite) surface of the plane
    */
   scale_vec2(Move, line.direction, D_proj/N_proj);

   /* Obtain the coordinates where the line intersects with the plane */
   add_vec(Delta, line.origin, Move);

   /* Store result */
   *translation = D_proj/N_proj;

   cross[0] = Delta[0];
   cross[1] = Delta[1];
   cross[2] = Delta[2];
   
   return 0;
}

int intersect_rect(const g_line line,
                   const g_rectangle rectangle,
                   double *translation, int *hit,
                   double *local_1, double *local_2)
{
   int    intersect;
   int    hit_val;
   vec3   cross;
   double path;
   vec3   O;
   double len1, len2;

   /* Find coordinate where particle crosses the plane that rectangle is part of */
   g_plane plane;
   copy_vec(plane.origin, rectangle.origin);
   copy_vec(plane.normal, rectangle.normal);

   intersect = intersect_plane(line, plane,
                               &path, cross);
   if (0 != intersect)
   {
      /* No single point found */
      return intersect;
   }

   /* Find the offset from the origin of the rectangle (from origin to crossing) */
   diff_vec(O, cross, rectangle.origin);

   /* Decompose vector into local edge coordinates */
   len1 = dot_vec(O, rectangle.edge1);
   len2 = dot_vec(O, rectangle.edge2);

   /* Limit hits to 'inside' coordinates: Length in either direction */
   hit_val = 0;
   if (fabs(len1) <= rectangle.edge1_len)
   {
      if (fabs(len2) <= rectangle.edge2_len)
      {
         hit_val = 1;
      }
   }

   /* Store the result */
   *translation = path;
   *hit = hit_val;
   *local_1 = len1;
   *local_2 = len2;

   return 0;
}

int intersect_box(const g_line line,
                  const g_box box,
                  double translation[2], int *hit,
                  double local_1[2], double local_2[2], double local_3[2])
{
   int     l_hit = 0;
   int     result = 0;
   int     h_count = 0;
   double  trans_res, l1, l2, l3;
   vec3    shift_side;
   vec3    r_side;

   /* Test the six sides of the box, using each 'edge' as a normal to the rectangle */
   /* Normal edge 1, shifted */
   scale_vec2(shift_side, box.edge1, box.edge1_len);
   add_vec(r_side, shift_side, box.origin);

   /* Hold temporary rectangle description */
   g_rectangle cur_rect;
   
   copy_vec(cur_rect.origin, r_side);
   copy_vec(cur_rect.normal, box.edge1);
   copy_vec(cur_rect.edge1, box.edge2);
   copy_vec(cur_rect.edge2, box.edge3);
   cur_rect.edge1_len = box.edge2_len;
   cur_rect.edge2_len = box.edge3_len;

   result = intersect_rect(line, cur_rect,
                           &trans_res, &l_hit, &l2, &l3);
   if (result == 0)
   {
      if (l_hit)
      {
         translation[h_count] = trans_res;
         local_1[h_count] = box.edge1_len;
         local_2[h_count] = l2;
         local_3[h_count] = l3;
         ++h_count;
      }
   }
   scale_vec2(shift_side, box.edge1, -box.edge1_len);
   add_vec(r_side, shift_side, box.origin);

   copy_vec(cur_rect.origin, r_side);
   /* Same as above
   copy_vec(cur_rect.normal, box.edge1);
   copy_vec(cur_rect.edge1, box.edge2);
   copy_vec(cur_rect.edge2, box.edge3);
   cur_rect.edge1_len = box.edge2_len;
   cur_rect.edge2_len = box.edge3_len;
   */
   result = intersect_rect(line, cur_rect,
                           &trans_res, &l_hit, &l3, &l3);
   if (result == 0)
   {
      if (l_hit)
      {
         translation[h_count] = trans_res;
         local_1[h_count] = -box.edge1_len;
         local_2[h_count] = l2;
         local_3[h_count] = l3;
         ++h_count;
      }
   }

   /* Normal edge 2, shifted */
   scale_vec2(shift_side, box.edge2, box.edge2_len);
   add_vec(r_side, shift_side, box.origin);

   copy_vec(cur_rect.origin, r_side);
   copy_vec(cur_rect.normal, box.edge2);
   copy_vec(cur_rect.edge1, box.edge3);
   copy_vec(cur_rect.edge2, box.edge1);
   cur_rect.edge1_len = box.edge3_len;
   cur_rect.edge2_len = box.edge1_len;

   result = intersect_rect(line, cur_rect,
                           &trans_res, &l_hit, &l3, &l1);
   if (result == 0)
   {
      if (l_hit)
      {
         translation[h_count] = trans_res;
         local_1[h_count] = l1;
         local_2[h_count] = box.edge2_len;
         local_3[h_count] = l3;
         ++h_count;
      }
   }

   scale_vec2(shift_side, box.edge2, -box.edge2_len);
   add_vec(r_side, shift_side, box.origin);

   copy_vec(cur_rect.origin, r_side);
   /* Same as above
   copy_vec(cur_rect.normal, box.edge2);
   copy_vec(cur_rect.edge1, box.edge3);
   copy_vec(cur_rect.edge2, box.edge1);
   cur_rect.edge1_len = box.edge3_len;
   cur_rect.edge2_len = box.edge1_len;
   */
   result = intersect_rect(line, cur_rect,
                           &trans_res, &l_hit, &l3, &l1);
   if (result == 0)
   {
      if (l_hit)
      {
         translation[h_count] = trans_res;
         local_1[h_count] = l1;
         local_2[h_count] = -box.edge2_len;
         local_3[h_count] = l3;
         ++h_count;
      }
   }

   /* Normal edge 3, shifted */
   scale_vec2(shift_side, box.edge3, box.edge3_len);
   add_vec(r_side, shift_side, box.origin);

   copy_vec(cur_rect.origin, r_side);
   copy_vec(cur_rect.normal, box.edge3);
   copy_vec(cur_rect.edge1, box.edge1);
   copy_vec(cur_rect.edge2, box.edge2);
   cur_rect.edge1_len = box.edge1_len;
   cur_rect.edge2_len = box.edge2_len;

   result = intersect_rect(line, cur_rect,
                           &trans_res, &l_hit, &l1, &l2);
   if (result == 0)
   {
      if (l_hit)
      {
         translation[h_count] = trans_res;
         local_1[h_count] = l1;
         local_2[h_count] = l2;
         local_3[h_count] = box.edge3_len;
         ++h_count;
      }
   }

   scale_vec2(shift_side, box.edge3, -box.edge3_len);
   add_vec(r_side, shift_side, box.origin);

   copy_vec(cur_rect.origin, r_side);
   /* Same as above
   copy_vec(cur_rect.normal, box.edge3);
   copy_vec(cur_rect.edge1, box.edge1);
   copy_vec(cur_rect.edge2, box.edge2);
   cur_rect.edge1_len = box.edge1_len;
   cur_rect.edge2_len = box.edge2_len;
   */
   result = intersect_rect(line, cur_rect,
                           &trans_res, &l_hit, &l1, &l2);
   if (result == 0)
   {
      if (l_hit)
      {
         translation[h_count] = trans_res;
         local_1[h_count] = l1;
         local_2[h_count] = l2;
         local_3[h_count] = -box.edge3_len;
         ++h_count;
      }
   }

   *hit = h_count;
   return 0;
}
