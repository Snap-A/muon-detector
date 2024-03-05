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

#include <math.h>

#include "pdg/pdg.h"
#include "vector/vector.h"
#include "sphere/sphere.h"

/* Constants */
const double pi = 3.14159265358979323846;

double val_omega(double th, double phi, const vec3 N, int flux,
                 double delTh, double delPhi)
{
   double r_vis;
   vec3   v_in;
   double delO;

   /* Direction to Omega element from origin. phi := 0 => x-axis */
   v_in[x_c] = cos(phi)*sin(th);
   v_in[y_c] = sin(phi)*sin(th);
   v_in[z_c] = cos(th);

   /* Omega element size */
   delO = sin(th) * delTh * delPhi;

   /* dot-product gives foreshortening ratio */
   r_vis = dot_vec(N, v_in);

   /* Return */
   return j_val(flux, th, phi) * fabs(r_vis) * delO;
}
