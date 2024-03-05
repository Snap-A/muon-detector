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

#include <stdlib.h>
#include "pdf/pdf.h"
#include "gun/gun.h"

static double s_min = 0.0;
static double s_max = 1.0;

static int gun_range(double* out)
{
   *out = s_min + (s_max-s_min) * drand48();
   return 0;
}

gun_ctx gun_range_init(double min, double max)
{
   gun_ctx context = gun_init(1);

   /* Save parameters */
   s_min = min;
   s_max = max;
   
   /* Create p.d.f for the a range of values */
   gun_config(context, 0, gun_range);

   return context;
}
