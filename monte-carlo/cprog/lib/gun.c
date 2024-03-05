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
#include <stdlib.h>
#include <string.h>

#include "sphere/sphere.h"
#include "gun/gun.h"

typedef struct gun_context
{
   int num_params;
   gun_trans t_array[];
} gct;

gun_ctx gun_init(int num_params)
{
   gct *new_ctx = malloc(sizeof(int)+num_params*sizeof(gun_trans));
   void* ct = (void*)new_ctx;
   memset(ct, 0, sizeof(int)+num_params*sizeof(gun_trans));
   new_ctx->num_params = num_params;
   return ct;
}

int gun_config(gun_ctx gt, int idx, gun_trans tr)
{
   gct *ctx = (gct*)gt;

   if ((idx < 0) || (idx >= ctx->num_params))
      return -1;

   ctx->t_array[idx] = tr;
   return 0;
}

int gun_event(gun_ctx gt, double *out)
{
   int idx;
   gct *ctx = (gct*)gt;

   if (NULL == out)
      return -1;

   for (idx = 0; idx < ctx->num_params; idx++)
      if (0 != (ctx->t_array[idx])(out+idx))
         return -1;

   return 0;
}

void gun_delete(gun_ctx gt)
{
   gct *old_ctx = (gct*)gt;
   free(old_ctx);
}
