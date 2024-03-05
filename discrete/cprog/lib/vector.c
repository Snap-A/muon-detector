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

#include "vector/vector.h"

/* Constants */
const int x_c = 0;
const int y_c = 1;
const int z_c = 2;

void copy_vec(vec3 out, const vec3 in)
{
   out[0] = in[0];
   out[1] = in[1];
   out[2] = in[2];
}

void scale_vec(vec3 v, double s)
{
   v[0] = s*v[0];
   v[1] = s*v[1];
   v[2] = s*v[2];
}

void scale_vec2(vec3 out, const vec3 in, double s)
{
   out[0] = s*in[0];
   out[1] = s*in[1];
   out[2] = s*in[2];
}

void diff_vec(vec3 out, const vec3 left, const vec3 right)
{
   out[0] = left[0] - right[0];
   out[1] = left[1] - right[1];
   out[2] = left[2] - right[2];
}

void add_vec(vec3 out, const vec3 left, const vec3 right)
{
   out[0] = left[0] + right[0];
   out[1] = left[1] + right[1];
   out[2] = left[2] + right[2];   
}

double dot_vec(const vec3 v1, const vec3 v2)
{
   return (v1[0]*v2[0]) + (v1[1]*v2[1]) + (v1[2]*v2[2]);
}

void cross_vec(vec3 out, const vec3 a, const vec3 b)
{
   out[x_c] = a[y_c]*b[z_c] - a[z_c]*b[y_c];
   out[y_c] = a[z_c]*b[x_c] - a[x_c]*b[z_c];
   out[z_c] = a[x_c]*b[y_c] - a[y_c]*b[x_c]; 
}

void mul_matrix(vec3 x, const mat33 M, const vec3 y)
{
   x[0] = M[0][0]*y[0] + M[0][1]*y[1] + M[0][2]*y[2];
   x[1] = M[1][0]*y[0] + M[1][1]*y[1] + M[1][2]*y[2];
   x[2] = M[2][0]*y[0] + M[2][1]*y[1] + M[2][2]*y[2];   
}
