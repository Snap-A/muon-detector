#include "pdf/pdf.h"
#include "gun/gun.h"

static double s_lambda = 0.0;

static int gun_decay(double* out)
{
   return decay_pdf(s_lambda, out);
}

gun_ctx gun_decay_init(double lambda)
{
   gun_ctx context = gun_init(1);

   /* Save parameter */
   s_lambda = lambda;
   
   /* Create p.d.f for the decay time */
   gun_config(context, 0, gun_decay);

   return context;
}
