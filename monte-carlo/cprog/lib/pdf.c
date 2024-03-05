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
#include <stddef.h>
#include <stdlib.h>

#include "sphere/sphere.h"
#include "pdf/pdf.h"

int uniform_pdf(double min, double max, double *out)
{
   if (NULL == out)
      return -1;

   *out = min + (max - min) * drand48();
   return 0;
}

int decay_pdf(double lambda, double *out)
{
   if (NULL == out)
      return -1;

   *out = -lambda * log(1.0 - drand48());
   return 0;
}

int isotropic_sphere_pdf(double *cos_t, double *phi)
{
   if ((NULL == cos_t)||(NULL == phi))
      return -1;

   *phi = 2.0 * pi * drand48(); /* Uniform between 0 and 2 pi */
   *cos_t = 1.0 - (2.0*drand48()); /* dOmega between 0 and pi */
   return 0;
}

int pdg_sphere_pdf(double *cos_t, double *phi)
{
   if ((NULL == cos_t)||(NULL == phi))
      return -1;

   *phi = 2.0 * pi * drand48(); /* Uniform between 0 and 2 pi */
   *cos_t = pow(drand48(), 0.33333333); /* dOmega between 0 and pi/2 */
   return 0;
}
