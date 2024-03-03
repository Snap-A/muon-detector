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
