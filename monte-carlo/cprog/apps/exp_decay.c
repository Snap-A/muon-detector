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

/**********************************************************/
/* Run the half-life decay simulation and record the      */
/* distribution of the generated decay time.              */
/**********************************************************/

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sphere/sphere.h"
#include "pdf/pdf.h"
#include "gun/gun.h"
#include "gun/gun_decay.h"

/* Prototypes */
static void usage(const char* name);

/* Implementations */
static void usage(const char* name)
{
   printf("Usage:\n%s [-e <num>] [-h] [-l <lambda>] <data file time>\n", name);
   printf("\n-- Options:\n");
   printf("-e <num> : Set the created events. Default is 100,000.\n");
   printf("-h       : Print this help text.\n");
   printf("-l <num> : Set the decay time (lambda) [sec]. Default is 10.0 sec.\n");
   printf("\n-- Positional arguments:\n");
   printf("<data file time>: Path to the data file to which the bin values for the decay times are written.\n");
}

/* Main */
int main(int argc, char *argv[])
{
   double time;
   double lambda;
   int    i, b;
   FILE   *f_outT;

   int index, type;
   int c;

   int total = 100000;
   int bins = 20;

   opterr = 0;
   lambda = 10.0;
   while ((c = getopt (argc, argv, "e:hl:")) != -1)
      switch (c)
      {
      case 'b':
         bins = atoi(optarg);
         printf("Set bin #: %d\n", bins);
         break;
      case 'e':
         total = atoi(optarg);
         printf("Set event #: %d\n", total);
         break;
      case 'h':
         usage(argv[0]);
         return 0;
      case 'l':
         lambda = strtod(optarg, NULL);
         printf("Set lambda: %e\n", lambda);
         break;
      case '?':
         if (strchr("el", optopt) != 0)
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
   if (((argc - optind) < 1) || ((argc - optind) > 1))
   {
      fprintf(stderr, "Incorrect number of arguments: %d\n", optind - argc);
      usage(argv[0]);
      return 1;
   }

   /* Read positional arguments */
   f_outT = fopen(argv[optind], "w");
   if (f_outT == NULL)
   {
      fprintf(stderr, "Could not open %s\n", argv[1]);
      return 1;
   }
   
   unsigned long *bins_t = (unsigned long*)malloc(sizeof(unsigned long)*bins);
   memset(bins_t, 0, sizeof(unsigned long)*bins);

   /* Scale transforming 0 <-> 1 binning limits to 0 <-> x_max value limits for x; x = time */
   double t_scale = 100.0;
   gun_ctx context = gun_decay_init(lambda);
   
   for (i=0; i < total; ++i)
   {
      /* Get new event data */
      if (0 != gun_event(context, &time))
      {
         fprintf(stderr, "PDF Failure!\n");
         return 1;
      }

      /* Add bin count in theta */
      b = (int)floor((double)bins * (time/t_scale));
      if ((0 <= b)&&(b <= bins-1))
         ++bins_t[b];
   }

   gun_delete(context);

   /* Document source */
   fprintf(f_outT, "# Created by 'exp_decay'\n# Parameters\n");
   fprintf(f_outT, "# BINS:    %d\n", bins);
   fprintf(f_outT, "# LAMBDA:  %e\n", lambda);
   fprintf(f_outT, "# TOTAL:   %d\n", total);

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

   /* Close files */
   if (f_outT != NULL)
      fclose(f_outT);

   return 0;
}
