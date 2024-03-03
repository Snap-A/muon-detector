#ifndef PDF_H_
#define PDF_H_

extern int uniform_pdf(double min, double max, double *out);
extern int decay_pdf(double lambda, double *out);
extern int isotropic_sphere_pdf(double *theta, double *phi);
extern int pdg_sphere_pdf(double *theta, double *phi);
#endif
