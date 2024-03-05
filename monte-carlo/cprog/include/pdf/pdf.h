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

#ifndef PDF_H_
#define PDF_H_

extern int uniform_pdf(double min, double max, double *out);
extern int decay_pdf(double lambda, double *out);
extern int isotropic_sphere_pdf(double *theta, double *phi);
extern int pdg_sphere_pdf(double *theta, double *phi);
#endif
