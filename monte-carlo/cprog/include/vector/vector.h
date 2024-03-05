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

#ifndef VECTOR_H_
#define VECTOR_H_

/* Constants */
extern const int x_c;
extern const int y_c;
extern const int z_c;

/* Types */
typedef double vec3[3];
typedef double mat33[3][3];

extern void copy_vec(vec3 out, const vec3 in);

extern void scale_vec(vec3 v, double s);
extern void scale_vec2(vec3 out, const vec3 in, double s);

extern void diff_vec(vec3 out, const vec3 left, const vec3 right);

extern void add_vec(vec3 out, const vec3 left, const vec3 right);

extern double dot_vec(const vec3 v1, const vec3 v2);

extern void cross_vec(vec3 out, const vec3 a, const vec3 b);

extern void mul_matrix(vec3 x, const mat33 M, const vec3 y);
#endif /* VECTOR_H_ */
