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

#include "pdf/pdf.h"
#include "gun/gun.h"
#include "sphere/sphere.h"
#include <stdlib.h>
#include <math.h>

static int gUseCos = 0;

static int gun_phi(double* out)
{
   *out = 2.0 * pi * drand48();
   return 0;
}

static int gun_theta(double* out)
{
   double cost = 1.0 - (2.0*drand48());
   if (gUseCos)
      *out = cost;
   else
      *out = acos(cost);
   return 0;
}

gun_ctx gun_iso_init(int use_cos)
{
   gun_ctx context = gun_init(2);

   /* Create p.d.f for spherical isotropic events */
   gun_config(context, 0, gun_theta);
   gun_config(context, 1, gun_phi);

   /* Save switch */
   gUseCos = use_cos;

   return context;
}
