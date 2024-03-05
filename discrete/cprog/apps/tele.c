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

/********************************************************/
/* Calculate the numerical approximation for the total  */
/* rate of cosmic munons for the given intensity        */
/* distribution through a muon 'telescope' made from    */
/* two rectangular detector surfaces with a distance    */
/* of 's' and pointing with the angle theta away from   */
/* the Zenith                                           */
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
   printf("Usage:\n%s [options] <delta theta> <delta phi> <delta x> <delta y> [<theta>]\n", name);
   printf("\n-- Options:\n");
   printf("-f <num>:  Select flux model. Default is 0\n");
   printf("           0 - PDG at sea-level\n");
   printf("           1 - Isotropic\n");
   printf("           2 - Point source at Zenith. Gaussian / sigma = 0.02 rad\n");
   printf("-h: Print this help\n");
   printf("-i: Instead of rates, print the coordinates of 'hits' (spherical coordinates)\n");
   printf("-l <length>: The longer size of the rectangle [m]. Default is 0.1 m\n");
   printf("-s <length>: The separation between the detectors [m]. Default is 1 m\n");
   printf("-w <length>: The shorter size of the rectangle [m]. Default is 0.1 m\n");
   printf("\n-- Positional arguments:\n");
   printf("<delta theta>: This value can be given as integer or float number.\n");
   printf("                <integer>: Divisor setting the size for the polar angle element (pi / 2N).\n");
   printf("                <float>:   Size of element in polar angle [radians]\n");
   printf("<delta phi>:   This value can be given as integer or float number.\n");
   printf("                <integer>: Divisor setting the size for the azimuth angle element (2pi / N).\n");
   printf("                <float>:   Size of element in azimuth angle [radians]\n");
   printf("<delta x>:     This value can be given as integer or float number.\n");
   printf("                <integer>: Divisor setting the size for the detector element long edeg (l / N).\n");
   printf("                <float>:   Size of detector element along the longer edge [m]\n");
   printf("<delta y>:     This value can be given as integer or float number.\n");
   printf("                <integer>: Divisor setting the size for the detector element short edge (w / N).\n");
   printf("                <float>:   Size of detector element along the shorter edge [m]\n");
   printf("<theta>:       Angle to zenith [radians]. Default is 0.\n");
}

#define DELTA_DEBUG 0

static int
val_delta(double xVal, double yVal,
          const vec3 O_in,
          double length, double width, double separation,
          const vec3 N, const vec3 X, const vec3 Y)
{
   double N_proj;
   double X_proj;
   double Y_proj;
   vec3   O;
   vec3   hit_2;
   vec3   D_proj;
   vec3   X_move;
   vec3   Y_move;
   
#if DELTA_DEBUG
   printf("     (%e)  \t   (%e)\n", O_in[x_c], N[x_c]);
   printf("O_in=(%e)  \t N=(%e)\n", O_in[y_c], N[y_c]);
   printf("     (%e)  \t   (%e)\n", O_in[z_c], N[z_c]);
   printf("----------------------------\n");
#endif
   /* Copy to scratch */
   copy_vec(O, O_in);

   /* Set displacement from detector origin */
   scale_vec2(X_move, X, xVal);
   scale_vec2(Y_move, Y, yVal);
   
   /* Project onto Normal to obtain scale to increase dOmega to intersect detector 2 */
   N_proj = dot_vec(O, N);
#if DELTA_DEBUG
   printf("N_pro =%e\n", N_proj);
#endif

#if 0
   /* Pointing away from detector 2? Stop */
   if (N_proj <= 0.0)
   {
#if DELTA_DEBUG
      printf("MISS - N_pro\n");
#endif
      return 0;
   }
#endif

   /* Scale vector (a) */
   scale_vec(O, 1.0/N_proj);

#if DELTA_DEBUG
   printf("    (%e)  \t     (%e)  \t   (%e)\n", X_move[x_c], Y_move[x_c], O[x_c]);
   printf("X_m=(%e)  \t Y_m=(%e)  \t O=(%e)\n", X_move[y_c], Y_move[y_c], O[y_c]);
   printf("    (%e)  \t     (%e)  \t   (%e)\n", X_move[z_c], Y_move[z_c], O[z_c]);
   printf("----------------------------\n");
#endif
   
   /* Project onto detector 1 by removing N from O */
   diff_vec(D_proj, O, N);

   /* Scale vector (b) to obtain projection at distance to detector 2 */
   scale_vec2(hit_2, D_proj, separation);

   /* Shift the origin per x,y data */
   add_vec(hit_2, hit_2, X_move);
   add_vec(hit_2, hit_2, Y_move);

#if DELTA_DEBUG
   printf("      (%e)  \t     (%e)\n", D_proj[x_c], hit_2[x_c]);
   printf("D_pro=(%e)  \t hit=(%e)\n", D_proj[y_c], hit_2[y_c]);
   printf("      (%e)  \t     (%e)\n", D_proj[z_c], hit_2[z_c]);
   printf("----------------------------\n");
#endif

   /* Project onto X and Y vectors in local coord */
   X_proj = dot_vec(hit_2, X);
   Y_proj = dot_vec(hit_2, Y);
#if DELTA_DEBUG
   printf("X_pro =%e    Y_pro =%e\n", X_proj, Y_proj);
#endif
   
   if (fabs(X_proj) > length/2.0)
   {
#if DELTA_DEBUG
      printf("MISS\n");
#endif
      return 0;
   }
   if (fabs(Y_proj) > width/2.0)
   {
#if DELTA_DEBUG
      printf("MISS\n");
#endif
      return 0;
   }

#if DELTA_DEBUG
   printf("HIT\n");
#endif
   return 1;
}

/* Main */
int main(int argc, char *argv[])
{
   double delT, delP;
   double delX, delY;
   double valT, valP;
   double valX, valY;
   double maxT, maxP;
   int intTheta, intPhi;
   int argTIsInt, argPIsInt;
   int intX, intY;
   int argXIsInt, argYIsInt;

   double valTot, valTot1, delA;
   double theta;
   vec3   v_ref;
   vec3   x_ref;
   vec3   y_ref;

   int index, type;
   int c;

   int flux = 0;
   int show_hits = 0;
   double length = 0.1;
   double width = 0.1;
   double separation = 1.0;
   double stop_theta = pi/2.0;

   opterr = 0;
   while ((c = getopt (argc, argv, "f:hil:s:w:")) != -1)
      switch (c)
      {
      case 'f':
         flux = atoi(optarg);
         break;
      case 'i':
         show_hits = 1;
         stop_theta = pi;
         break;
      case 'h':
         usage(argv[0]);
         return 0;
      case 'l':
         length = strtod(optarg, NULL);
         break;
      case 's':
         separation = strtod(optarg, NULL);
         break;
      case 'w':
         width = strtod(optarg, NULL);
         break;
      case '?':
         if (strchr("flsw", optopt) != 0)
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
   if (((argc - optind) < 4) || ((argc - optind) > 5))
   {
      fprintf(stderr, "Incorrect number of arguments: %d\n", optind - argc);
      usage(argv[0]);
      return 1;
   }

   /* Read positional arguments */
   delT = delP = delX = delY = 0.0;
   theta = 0.0;

   maxT = stop_theta;
   maxP = 2.0*pi;

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
         if (NULL == strchr(argv[index], '.'))
         {
            argXIsInt = 1;
            intX = atoi(argv[index]);
            delX = length / (double)intX;
         }
         else
         {
            argXIsInt = 0;
            delX = strtod(argv[index], NULL);
         }
         break;

      case 3:
         if (NULL == strchr(argv[index], '.'))
         {
            argYIsInt = 1;
            intY = atoi(argv[index]);
            delY = width / (double)intY;
         }
         else
         {
            argYIsInt = 0;
            delY = strtod(argv[index], NULL);
         }
         break;

      case 4:
         theta = strtod(argv[index], NULL);
         break;
      }

      ++type;
   }

   if ((delT == 0.0)||(delP == 0.0)||(delX == 0.0)||(delY == 0.0))
   {
      fprintf(stderr, "All delta values must be non-zero: (%g / %g), (%g / %g)\n",
              delT, delP, delX, delY);
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

   /* Adjust delX and delY so that no area is left out */
   if (!argXIsInt)
   {
      intX = (int)floor(length/delX);
      delX = length / (double)intX;
   }

   if (!argYIsInt)
   {
      intY = (int)floor(width/delY);
      delY = width / (double)intY;
   }
   
   valTot1 = 0.0;
   valTot = 0.0;

   /* Detector is rotated along 'x' by the given angle theta */
   /* Rotational matrix would be:                            */
   /*                              | 1    0    0  |          */
   /*                    R(theta)= | 0   cos  sin |          */
   /*                              | 0  -sin  cos |          */
   /* -----------------------------------------------------  */
   /* Applied to coordinate vectors: X, Y, Z                 */
   /*    X => x_ref                                          */ 
   /*    Y => y_ref                                          */ 
   /*    Z => v_ref                                          */ 

   x_ref[x_c] = 1.0;
   x_ref[y_c] = 0.0;
   x_ref[z_c] = 0.0;

   y_ref[x_c] = 0.0;
   y_ref[y_c] = cos(theta);
   y_ref[z_c] = sin(theta);

   v_ref[x_c] = 0.0;
   v_ref[y_c] = -1.0*sin(theta);
   v_ref[z_c] = cos(theta);


   /* Area element size */
   delA = delX * delY;

   /* valT and valP are taken in the fixed spherical coordinates of 'Earth' */
   /* with (valT) relative to the Zenith and (valP) the Azimuth */
   for (valT = delT / 2.0; valT < stop_theta; valT += delT)
   {
      for (valP = delP / 2.0; valP < maxP; valP += delP)
      {
         double addTot;
         vec3   O;
         int    out = 0;

         /* Create unit vector for the dOmega direction (Earth coord) */
         O[x_c] = cos(valP)*sin(valT);
         O[y_c] = sin(valP)*sin(valT);
         O[z_c] = cos(valT);

         /* Intensity value for this dOmega direction */
         addTot = val_omega(valT, valP, v_ref, flux, delT, delP) * delA;

         if (show_hits)
         {
            /* Only prints 'hits' for center of detector 1 */
            valX = valY = 0;
            if (val_delta(valX, valY, O, length, width, separation, v_ref, x_ref, y_ref))
               printf("%e %e %e\n", valT, valP, addTot);
         }
         else
         {
            /* valX and valY are taken at the local coordinates of detector 1 */
            for (valX = (delX/2.0)-(length/2.0); valX < length/2.0; valX += delX)
            {
               for (valY = (delY/2.0)-(width/2.0); valY < width/2.0; valY += delY)
               {
                  /* Single detector value */
                  valTot1 += addTot;
               
                  /* Only add when O is also crossing detector 2 */
                  if (val_delta(valX, valY, O, length, width, separation, v_ref, x_ref, y_ref))
                  {
                     valTot += addTot;
                  }
               }
            }
         }
      }
   }

   if (!show_hits)
   {
      printf("Total det 1 = %e\n", valTot1);
      printf("Total both  = %e\n", valTot);
   }
}
