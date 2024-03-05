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

#ifndef GUN_H_
#define GUN_H_

typedef void *gun_ctx;
typedef int (*gun_trans)(double* out);

extern gun_ctx gun_init(int num_params);
extern int gun_config(gun_ctx gt, int idx, gun_trans tr);
extern int gun_event(gun_ctx gt, double *out);
extern void gun_delete(gun_ctx gt);

#endif /* GUN_H_ */
