#include "pdf/pdf.h"
#include "gun/gun.h"
#include "sphere/sphere.h"
#include <stdlib.h>
#include <math.h>

static int gUseCos = 0;

static int gun_phi(double* out)
{
   *out = 2.0 * pi * drand48();
   return 0;
}

static int gun_theta(double* out)
{
   double cost = 1.0 - (2.0*drand48());
   if (gUseCos)
      *out = cost;
   else
      *out = acos(cost);
   return 0;
}

gun_ctx gun_iso_init(int use_cos)
{
   gun_ctx context = gun_init(2);

   /* Create p.d.f for spherical isotropic events */
   gun_config(context, 0, gun_theta);
   gun_config(context, 1, gun_phi);

   /* Save switch */
   gUseCos = use_cos;

   return context;
}
