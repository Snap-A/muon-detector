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

#ifndef PDG_H_
#define PDG_H_

/* Vertical Muon intensity at sea level - PDG */
extern const double mu_pdg_i;

/* Vertical Muon intensity at sea level - Assumed Isotropic */
extern const double mu_iso_i;

/**************************************************/
/* Return the muon intensity for a given set of   */
/* angles: \theta = Polar angle                   */
/*         \phi   = Azimuth angle                 */
/**************************************************/

/* Select one of the distributions */
extern double j_val(int type, double theta, double phi);

/* PDG intensity */
extern double j_val_PDG(double theta, double phi);

/* Isotropic intensity */
extern double j_val_ISO(double theta, double phi);

/* Point source at Zenith */
extern double j_val_ZEN(double theta, double phi);

/* Total rate of PDG flux through an area with rotated angle from zenith */
extern double r_tot_PDG(double theta, double area);

#endif /* PDG_H_ */
