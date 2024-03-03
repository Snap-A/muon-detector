#ifndef SPHERE_H_
#define SPHERE_H_

#include "vector/vector.h"

/* Constants */
extern const double pi;

extern double val_omega(double th, double phi, const vec3 N, int flux,
                        double delTh, double delPhi);
#endif /* SPHERE_H_ */
