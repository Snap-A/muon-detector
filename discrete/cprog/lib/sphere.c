#include <math.h>

#include "pdg/pdg.h"
#include "vector/vector.h"
#include "sphere/sphere.h"

/* Constants */
const double pi = 3.14159265358979323846;

double val_omega(double th, double phi, const vec3 N, int flux,
                 double delTh, double delPhi)
{
   double r_vis;
   vec3   v_in;
   double delO;

   /* Direction to Omega element from origin. phi := 0 => x-axis */
   v_in[x_c] = cos(phi)*sin(th);
   v_in[y_c] = sin(phi)*sin(th);
   v_in[z_c] = cos(th);

   /* Omega element size */
   delO = sin(th) * delTh * delPhi;

   /* dot-product gives foreshortening ratio */
   r_vis = dot_vec(N, v_in);

   /* Return */
   return j_val(flux, th, phi) * fabs(r_vis) * delO;
}
