/********************************************************/
/* Calculate the numerical approximation for the total  */
/* rate of cosmic munons for the given intensity        */
/* distribution for a single detector surface.          */
/********************************************************/

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "pdg/pdg.h"
#include "vector/vector.h"
#include "sphere/sphere.h"

/* Prototypes */
static void usage(const char* name);

/* Implementations */
static void usage(const char* name)
{
   printf("Usage:\n%s [-e] [-f <num>] <delta theta> <delta phi> [<theta>]\n", name);
   printf("\n-- Options:\n");
   printf("-e      :  Try to estimate systematic error.\n");
   printf("           When turned on, the flux error contribution will be estimated by determining\n");
   printf("           the variaton of the flux at the 'corners' of the element selected.\n");
   printf("-f <num>:  Select flux model. Default is 0\n");
   printf("           0 - PDG at sea-level\n");
   printf("           1 - Isotropic\n");
   printf("           2 - Point source at Zenith. Gaussian with sigma^2 = 0.02 [radians]\n");
   printf("\n-- Positional arguments:\n");
   printf("<delta theta>: This value can be given as integer or float number.\n");
   printf("                <integer>: Divisor setting the size for the polar angle element (pi / 2N).\n");
   printf("                <float>:   Size of element in polar angle [radians]\n");
   printf("<delta phi>:   This value can be given as integer or float number.\n");
   printf("                <integer>: Divisor setting the size for the azimuth angle element (2pi / N).\n");
   printf("                <float>:   Size of element in azimuth angle [radians]\n");
   printf("<theta>:       Angle to zenith [radians]. Default is 0.\n");
}

/* Main */
int main(int argc, char *argv[])
{
   double delT, delP;
   double valT, valP;
   double maxT, maxP;
   int intTheta, intPhi;
   int argTIsInt, argPIsInt;

   double valTot;
   double theta;
   vec3   v_ref;
   double valETot[4];

   int flux = 0;
   int estimate = 0;
   int index, type;
   int c;

   opterr = 0;
   while ((c = getopt (argc, argv, "ef:h")) != -1)
      switch (c)
      {
      case 'f':
         flux = atoi(optarg);
         break;
      case 'e':
         estimate = 1;
         break;
      case 'h':
         usage(argv[0]);
         return 0;
      case '?':
         if (strchr("f", optopt) != 0)
            fprintf (stderr, "Option -%c requires an argument.\n", optopt);
         else if (isprint (optopt))
            fprintf (stderr, "Unknown option `-%c'.\n", optopt);
         else
            fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
         return 1;
      default:
         abort ();
      }

   /* Total positional arguments */
   if (((argc - optind) < 2) || ((argc - optind) > 3))
   {
      fprintf(stderr, "Incorrect number of arguments: %d\n", optind - argc);
      usage(argv[0]);
      return 1;
   }

   maxT = pi/2.0;
   maxP = 2.0*pi;

   /* Read positional arguments */
   theta = 0.0;
   type = 0;
   for (index = optind; index < argc; index++)
   {
      switch(type)
      {
      case 0:
         if (NULL == strchr(argv[index], '.'))
         {
            argTIsInt = 1;
            intTheta = atoi(argv[index]);
            delT = maxT / (double)intTheta;
         }
         else
         {
            argTIsInt = 0;
            delT = strtod(argv[index], NULL);
         }
         break;

      case 1:
         if (NULL == strchr(argv[index], '.'))
         {
            argPIsInt = 1;
            intPhi = atoi(argv[index]);
            delP = maxP / (double)intPhi;
         }
         else
         {
            argPIsInt = 0;
            delP = strtod(argv[index], NULL);
         }
         break;

      case 2:
         theta = strtod(argv[index], NULL);
         break;
      }

      ++type;
   }

   if ((delT == 0.0)||(delP == 0.0))
   {
      fprintf(stderr, "Both delta values must be non-zero: (%g / %g)\n",
              delT, delP);
      usage(argv[0]);
      return 1;
   }

   /* Adjust delT and delP so that no area is left out */
   if (!argTIsInt)
   {
      intTheta = (int)floor(maxT/delT);
      delT = maxT / (double)intTheta;
   }

   if (!argPIsInt)
   {
      intPhi = (int)floor(maxP/delP);
      delP = maxP / (double)intPhi;
   }

   valTot = 0.0;
   valETot[0] = 0.0;
   valETot[1] = 0.0;
   valETot[2] = 0.0;
   valETot[3] = 0.0;

   /* Detector is tilted along 'x' by the given angle */
   v_ref[x_c] = 0.0;
   v_ref[y_c] = -1.0*sin(theta);
   v_ref[z_c] = cos(theta);
   
   /* valT and valP are taken in the fixed spherical coordinates of 'Earth' */
   /* with (valT) relative to Zenith */
   for (valT = delT / 2.0; valT < maxT; valT += delT)
   {
      for (valP = delP / 2.0; valP < maxP; valP += delP)
      {
         valTot += val_omega(valT, valP, v_ref, flux, delT, delP);;
         if (estimate)
         {
            valETot[0] += val_omega(valT-(delT/2.0), valP-(delP/2.0), v_ref, flux, delT, delP);
            valETot[1] += val_omega(valT-(delT/2.0), valP+(delP/2.0), v_ref, flux, delT, delP);
            valETot[2] += val_omega(valT+(delT/2.0), valP-(delP/2.0), v_ref, flux, delT, delP);
            valETot[3] += val_omega(valT+(delT/2.0), valP+(delP/2.0), v_ref, flux, delT, delP);
         }
      }
   }

   if (estimate)
   {
      int i;
      double average = valTot;
      double variance = 0.0;

      /* Sum of all values (1st moment) */
      for (i=0; i<4; ++i)
      {
         average += valETot[i];
      }
      /* Average value V_a = V / N */
      average = average / 5.0;

      /* Sum of all squared differences (2nd moment) */
      variance = (average - valTot)*(average - valTot);
      for (i=0; i<4; ++i)
      {
         variance += (average - valETot[i])*(average - valETot[i]);
      }

      /* Variance value s^2 = var(t) / (N-1) */
      variance = variance / 4.0;

      /* Value, variance, and estimated error */
      printf("Total = %e, s^2(Total) = %e, s(Total) = %e\n", valTot, variance, (float)sqrtl(variance));
   }
   else
   {
      printf("Total = %e\n", valTot);
   }
}
