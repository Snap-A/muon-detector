#include <math.h>
#include <stdlib.h>

#include "pdg/pdg.h"
#include "sphere/sphere.h"

/* Muon intensity at sea level per PDG */
const double mu_pdg_i = 70.0;  /* m^-2 s^-1 sr^-2 */

/* Muon intensity for isotropic distribution, scaled to the same total rate */
const double mu_iso_i = mu_pdg_i / 2.0;  /* m^-2 s^-1 sr^-2 */

/* Muon intensity for point source with 0.02 rads sigma */
const double mu_pnt_i = mu_pdg_i * (12.8369);  /* m^-2 s^-1 sr^-2 */

/* Implementation of the functions */

double j_val(int type, double theta, double phi)
{
   switch(type)
   {
   case 0: /* PDG */
      return j_val_PDG(theta, phi);
            
   case 1: /* Isotropic */
      return j_val_ISO(theta, phi);

   case 2: /* Point source */
      return j_val_ZEN(theta, phi);

   default:
      abort();
   }
}

double j_val_PDG(double theta, double phi)
{
   double ct = cos(theta);

   /* Cut off flux below horizon */
   if (ct > 0.0)
      return (mu_pdg_i * ct * ct);
   else
      return 0.0;
}

double j_val_ISO(double theta, double phi)
{
   double ct = cos(theta);

   /* Cut off flux below horizon */
   if (ct > 0.0)
      return mu_iso_i;
   else
      return 0.0;
}

double j_val_ZEN(double theta, double phi)
{
   double ct = cos(theta);
   double src = (theta*theta) / (2*0.02);

   /* Cut off flux below horizon */
   if (ct > 0.0)
      return mu_pnt_i * exp(-1.0*src);
   else
      return 0.0;
}

double r_tot_PDG(double theta, double area)
{
   const double ar_flux_z = mu_pdg_i * pi / 2.0;
   const double ar_flux_h = ar_flux_z / 2.0;
   const double center = (ar_flux_z + ar_flux_h) / 2.0;
   const double spread = (ar_flux_z - ar_flux_h) / 2.0;
   
   return (center + spread * cos(2.0 * theta)) * area;
}
