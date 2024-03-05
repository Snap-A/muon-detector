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

#ifndef GUN_ISO_H_
#define GUN_ISO_H_

#include "gun/gun.h"

typedef struct {
   double theta;
   double phi;
} iso_output_t;

typedef struct {
   double cos_theta;
   double phi;
} iso_output_c;

typedef union {
   double       pars[2];
   iso_output_t out_t;
   iso_output_c out_c;
} iso_event;

extern gun_ctx gun_iso_init(int use_cos);

#endif /* GUN_ISO_H_ */
