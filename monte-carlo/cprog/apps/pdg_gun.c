/**********************************************************/
/* Run the PDG particle "gun" and record the distribution */
/* of generated particle's angle theta and phi.           */
/**********************************************************/

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sphere/sphere.h"
#include "pdg/pdg.h"
#include "gun/gun.h"

/* Prototypes */
static void usage(const char* name);

/* Implementations */
static void usage(const char* name)
{
   printf("Usage:\n%s [-b <num>] [-c] [-e <num>] [-h] <data file theta> <data file phi>\n", name);
   printf("\n-- Options:\n");
   printf("-b <num> : Set number of bins. Default is 200.\n");
   printf("-c       : Switch to cos(theta) projection of events\n");
   printf("-e <num> : Set the created events. Default is 100,000.\n");
   printf("-h       :  Print this help text.\n");
   printf("\n-- Positional arguments:\n");
   printf("<data file theta>: Path to the data file to which the bin values for theta are written.\n");
   printf("<data file phi>  : Path to the data file to which the bin values for phi are written.\n");
}

/* Main */
int main(int argc, char *argv[])
{
   double cost, theta;
   double phi;
   int    i, b;
   FILE   *f_outT, *f_outP;

   int index, type;
   int c;

   int bins = 200;
   int total = 100000;
   int cos_theta = 0;

   opterr = 0;
   while ((c = getopt (argc, argv, "b:ce:h")) != -1)
      switch (c)
      {
      case 'b':
         bins = atoi(optarg);
         printf("Set bin #: %d\n", bins);
         break;
      case 'c':
         printf("Generate as cos(theta)\n");
         cos_theta = 1;
         break;
      case 'e':
         total = atoi(optarg);
         printf("Set event #: %d\n", total);
         break;
      case 'h':
         usage(argv[0]);
         return 0;
      case '?':
         if (strchr("f", optopt) != 0)
            fprintf(stderr, "Option -%c requires an argument.\n", optopt);
         else if (isprint (optopt))
            fprintf(stderr, "Unknown option `-%c'.\n", optopt);
         else
            fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
         return 1;
      default:
         abort();
      }
   
   /* Total positional arguments */
   if (((argc - optind) < 2) || ((argc - optind) > 2))
   {
      fprintf(stderr, "Incorrect number of arguments: %d\n", optind - argc);
      usage(argv[0]);
      return 1;
   }

   /* Set plot arrays */
   unsigned long *bins_t = (unsigned long*)malloc(sizeof(unsigned long)*bins);
   unsigned long *bins_p = (unsigned long*)malloc(sizeof(unsigned long)*bins);

   memset(bins_t, 0, sizeof(unsigned long)*bins);
   memset(bins_p, 0, sizeof(unsigned long)*bins);

   /* Read positional arguments */
   f_outT = NULL;
   f_outP = NULL;

   for (index = optind; index < argc; index++)
   {
      switch(type)
      {
      case 0:
         f_outT = fopen(argv[index], "w");
         break;

      case 1:
         f_outP = fopen(argv[index], "w");
         break;
      }
      ++type;
   }

   /* Scale transforming 0 <-> 1 binning limits to 0 <-> x_max value limits for x; x = [phi] or [theta] */
   double t_scale = pi/2.0;
   double p_scale = 2.0*pi;

   /* Adjust scale */
   if (1 == cos_theta) t_scale = 1.0;

   for (i=0; i < total; ++i)
   {
      /* Use PDG p.d.f for (Theta, Phi) */
      cost = pow(drand48(), 0.33333333333);
      theta = acos(cost);
      phi = drand48() * p_scale;

      if (0 == cos_theta)
      {
         /* Add bin count in theta */
         b = (int)floor((double)bins * (theta/t_scale));
         if ((0 <= b)&&(b <= bins-1))
            ++bins_t[b];
      }
      else
      {
         /* Add bin count in cos(theta) */
         b = (int)floor((double)bins * (cost/t_scale));
         if ((0 <= b)&&(b <= bins-1))
            ++bins_t[b];
      }

      /* Add bin count in phi */
      b = (int)floor((double)bins * (phi/p_scale));
      if ((0 <= b)&&(b <= bins-1))
         ++bins_p[b];
   }

   /* Document source */
   fprintf(f_outT, "# Created by 'pdg_gun'\n# Parameters\n");
   fprintf(f_outT, "# BINS:    %d\n", bins);
   fprintf(f_outT, "# COS_T:   %d\n", cos_theta);
   fprintf(f_outT, "# TOTAL:   %d\n", total);

   fprintf(f_outP, "# Created by 'pdg_gun'\n");
   fprintf(f_outP, "# BINS:    %d\n", bins);
   fprintf(f_outP, "# TOTAL:   %d\n", total);
   
   /* Print data */
   for (i=0; i < bins; ++i)
   {
      double x_low = t_scale*((double)i/(double)bins);
      double x_high = t_scale*((double)(i+1)/(double)bins);
      double y_low = (double)(bins_t[i]) - sqrt((double)bins_t[i]);
      double y_high = (double)(bins_t[i]) + sqrt((double)bins_t[i]);
      fprintf(f_outT, "%e\t%e\t%e\t%e\t%e\t%e\n", (x_low+x_high)/2.0, (double)bins_t[i],
              x_low, x_high, y_low, y_high);
   }

   for (i=0; i < bins; ++i)
   {
      double x_low = p_scale*((double)i/(double)bins);
      double x_high = p_scale*((double)(i+1)/(double)bins);
      double y_low = (double)(bins_p[i]) - sqrt((double)bins_p[i]);
      double y_high = (double)(bins_p[i]) + sqrt((double)bins_p[i]);
      fprintf(f_outP, "%e\t%e\t%e\t%e\t%e\t%e\n", (x_low+x_high)/2.0, (double)bins_p[i],
              x_low, x_high, y_low, y_high);
   }

   /* Close files */
   if (f_outT != NULL)
      fclose(f_outT);

   if (f_outP != NULL)
      fclose(f_outP);
   
   return 0;
}
