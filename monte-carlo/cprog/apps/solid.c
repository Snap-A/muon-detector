/***********************************************************/
/* Run single detector solid box simulation and record     */
/* the ratio of detected events over all generated events. */
/***********************************************************/

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sphere/sphere.h"
#include "pdg/pdg.h"
#include "vector/vector.h"
#include "geometry/geometry.h"
#include "pdf/pdf.h"
#include "gun/gun.h"
#include "gun/gun_iso.h"
#include "gun/gun_pdg.h"
#include "gun/gun_range.h"

/* Prototypes */
static void usage(const char* name);

/* Implementations */
static void usage(const char* name)
{
   printf("Usage:\n%s [-d <double>] [-e <num>] [-f <num>] [-h] [-l <double>] [-t >double>] [-w <double>] <theta>\n", name);
   printf("\n-- Options:\n");
   printf("-b <num>    : Set number of bins. Default is 100.\n");
   printf("-d <double> : Set the depth of the detector [m]. (Default is 0.01 m)\n");
   printf("-e <num>    : Set the number of events to simulate. (Default is 1,000,000)\n");
   printf("-f <num>    : Set the simulated flux of particle. (Default is 0 = PDG)\n");
   printf("              0 = PDG flux. (~ cos^2 theta)\n");
   printf("              1 = Isotropic flux.\n");
   printf("-h          : Print this help text.\n");
   printf("-l <double> : Set the (longer) length of the detector [m]. (Default is 0.1 m)\n");
   printf("-o <double> : Set the 'world' scaled length relative to detector length. (Default is 8.0)\n");
   printf("-p <type>   : Save histogram data for given type. (Default is 0 = 'none')\n");
   printf("              bit0 = hit (x,y) at center plane of detector (normal=up)\n");
   printf("              bit1 = track length inside of detector\n");
   printf("-t <double> : Set the minimal length of the track 'inside' the solid to count as a 'hit' [m]. (Default is 0.003)\n");
   printf("-u          : Disable 'foreshortening' rule on particles. (Default is to use it)\n");
   printf("-w <double> : Set the (shorter) width of the detector [m]. (Default is 0.1 m)\n");
   printf("\n-- Positional arguments:\n");
   printf("<theta>          : Angle to zenith [radians].\n");
}

/* Main */
int main(int argc, char *argv[])
{
   const int bins_xy = 31;
   const double total_rate_per_m2 = mu_pdg_i * pi / 2.0; /* Hz/m^2 */

   const int plot_xy = 1<<0;
   const int plot_tr = 1<<1;

   double theta_d = 0.0;
   int    flux = 0;
   int    plot = 0;
   double world_scale = 8.0;
   int    total = 1000000;
   double depth = 0.01;
   double length = 0.1;
   double width = 0.1;
   double track = 0.003;
   int    use_f = 1;
   int bins     = 100;

   unsigned long bins_X_Y[bins_xy][bins_xy];

   int index, type;
   int c;

   opterr = 0;
   while ((c = getopt (argc, argv, "b:d:e:f:hl:o:p:t:uw:")) != -1)
      switch (c)
      {
      case 'b':
         bins = atoi(optarg);
         printf("Set bin #: %d\n", bins);
         break;
      case 'd':
         depth = strtod(optarg, NULL);
         break;
      case 'e':
         total = atoi(optarg);
         break;
      case 'f':
         flux = atoi(optarg);
         break;
      case 'h':
         usage(argv[0]);
         return 0;
      case 'l':
         length = strtod(optarg, NULL);
         break;
      case 'o':
         world_scale = strtod(optarg, NULL);
         break;
      case 'p':
         plot = atoi(optarg);
         break;
      case 't':
         track = strtod(optarg, NULL);
         break;
      case 'u':
         use_f = 0;
         break;
      case 'w':
         width = strtod(optarg, NULL);
         break;
      case '?':
         if (strchr("deflptw", optopt) != 0)
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

   /* Set plot arrays for 'tr' */
   unsigned long *bins_tr = (unsigned long*)malloc(sizeof(unsigned long)*bins);

   memset(bins_tr, 0, sizeof(unsigned long)*bins);
   
   /* Read positional arguments */
   type = 0;
   for (index = optind; index < argc; index++)
   {
      switch(type)
      {
      case 0:
         theta_d = strtod(argv[optind], NULL);
         break;
      }
      ++type;
   }

   /* Particle event data */
   g_line part;

   /* Detector box setup */
   g_box  box;

   /* World edge's normal */
   double w_n[3];
   /* World area rate */
   double rate_w;

   double flux_scale;

   gun_ctx contextI = NULL;
   gun_ctx contextL = NULL;
   gun_ctx contextW = NULL;

   if (flux == 0)
   {
      /* Set PDFs for particle gun */
      /* Return theta = 0, cos(theta) = 1 */
      contextI = gun_pdg_init(0);
      flux_scale = 1.0;
   }
   if (flux == 1)
   {
      /* Set PDFs for particle gun */
      /* Return theta = 0, cos(theta) = 1 */
      contextI = gun_iso_init(0);
      flux_scale = 1.0;
   }

   /* Return coordinate on 'world' edge = larger than solid size */
   contextL = gun_range_init(-length*(world_scale/2.0), length*(world_scale/2.0));
   contextW = gun_range_init(-width*(world_scale/2.0), width*(world_scale/2.0));

   rate_w = total_rate_per_m2 * (length * world_scale) * (width * world_scale);

   /* World's edge normal points up to zenith */
   w_n[x_c] = 0.0;
   w_n[y_c] = 0.0;
   w_n[z_c] = 1.0;

   /* Detector box setup */
   box.origin[x_c] = 0.0;
   box.origin[y_c] = 0.0;
   box.origin[z_c] = 0.0;

   /* Edge1 = Along x axis */
   box.edge1[x_c] = 1.0;
   box.edge1[y_c] = 0.0;
   box.edge1[z_c] = 0.0;

   /* Edge2 = Along y axis */
   box.edge2[x_c] = 0.0;
   box.edge2[y_c] = 1.0;
   box.edge2[z_c] = 0.0;

   /* Edge3 = Along z axis */
   box.edge3[x_c] = 0.0;
   box.edge3[y_c] = 0.0;
   box.edge3[z_c] = 1.0;

   /* Rotational axis with theta -> x-axis */
   double rot_axis[3] = { 1.0, 0.0, 0.0 };

   /* Rotate detector box edges */
   if (fabs(theta_d) > 1E-10)
   {
      double out[3];

      rotate_vec(out, box.edge1, rot_axis, theta_d);
      copy_vec(box.edge1, out);

      rotate_vec(out, box.edge2, rot_axis, theta_d);
      copy_vec(box.edge2, out);

      rotate_vec(out, box.edge3, rot_axis, theta_d);
      copy_vec(box.edge3, out);
   }

   box.edge1_len = length/2.0;
   box.edge2_len = width/2.0;
   box.edge3_len = depth/2.0;
   
   /* Hit counter */
   int    count = 0;

   /* Plot data output */
   FILE   *f_outXY    = NULL;
   FILE   *f_outTrans = NULL;

   double xy_scale    = 1.0;
   double tr_scale    = 1.0;

   if (plot & plot_tr)
   {
      memset(bins_tr, 0, sizeof(bins_tr));
      printf("Writing to 'trans.data'\n");
      f_outTrans = fopen("trans.data", "w");
      tr_scale = 1.05*sqrt((length*length)+(width*width)+(depth*depth));
   }

   if (plot & plot_xy)
   {
      memset(bins_X_Y, 0, sizeof(bins_X_Y));
      printf("Writing to 'xy_hit.data'\n");
      f_outXY = fopen("xy_hit.data", "w");
      xy_scale = 1.20*width;
   }

   int i;
   for (i=0; i < total; ++i)
   {
      iso_event evt_i;
      pdg_event evt_p;
      double    x_0, y_0;
      double    t, p;
      double    trans[2];
      double    l1[2], l2[2], l3[2];
      int       hit, retVal;

      /* Get new event data */
      if (flux == 0)
      {
         if (0 != gun_event(contextI, evt_p.pars))
         {
            fprintf(stderr, "PDG PDF Failure!\n");
            return 1;
         }
         t = evt_p.out_t.theta;
         p = evt_p.out_t.phi;
      }
      else if (flux == 1)
      {
         if (0 != gun_event(contextI, evt_i.pars))
         {
            fprintf(stderr, "ISO PDF Failure!\n");
            return 1;
         }
         t = evt_i.out_t.theta;
         p = evt_i.out_t.phi;
      }

      if (0 != gun_event(contextL, &x_0))
      {
         fprintf(stderr, "X0 PDF Failure!\n");
         return 1;
      }
      if (0 != gun_event(contextW, &y_0))
      {
         fprintf(stderr, "Y0 PDF Failure!\n");
         return 1;
      }

      /* Set particle values at world's edge */
      part.origin[x_c] = x_0;
      part.origin[y_c] = y_0;
      part.origin[z_c] = length+depth;

      /* Set particle direction */
      part.direction[x_c] = sin(t)*cos(p);
      part.direction[y_c] = sin(t)*sin(p);
      part.direction[z_c] = cos(t);

      if (use_f)
      {
         /* Obtain dot product to normal of world edge to enforce foreshortening effect */
         double f_size = fabs(dot_vec(w_n, part.direction));

         if (f_size < drand48())
         {
            /* Address over-density of angled particles due to foreshortening by rejecting this event */
            --i;
            continue;
         }
      }

      /* Find geometric intersection */
      retVal = intersect_box(part, box,
                             trans, &hit,
                             l1, l2, l3);

      if (retVal == 0)
      {
         if (hit)
         {
            double track_len = fabs(trans[0] - trans[1]);
            if (track_len > track)
               count++;
            else
               continue;

            if (plot & plot_tr)
            {
               /* Add bin count in theta */
               int    b;

               b = (int)floor((double)bins * (track_len/tr_scale));
               if ((0 <= b)&&(b <= bins-1))
                  ++bins_tr[b];
            }

            if (plot & plot_xy)
            {
               /* Add bin count in X/Y plane */
               int    x_b, y_b;

               x_b = (int)floor((double)bins_xy * ((l1[0]+(xy_scale/2.0))/xy_scale));
               y_b = (int)floor((double)bins_xy * ((l2[0]+(xy_scale/2.0))/xy_scale));

               if ((0 <= x_b)&&(x_b <= bins_xy-1) &&
                   (0 <= y_b)&&(y_b <= bins_xy-1))
                  ++bins_X_Y[x_b][y_b];
            }
         }
      }
   }

   if (f_outTrans != NULL)
   {
      /* Print data */
      for (i=0; i < bins; ++i)
      {
         double x_low = tr_scale*((double)i/(double)bins);
         double x_high = tr_scale*((double)(i+1)/(double)bins);
         double y_low = (double)(bins_tr[i]) - sqrt((double)bins_tr[i]);
         double y_high = (double)(bins_tr[i]) + sqrt((double)bins_tr[i]);
         fprintf(f_outTrans, "%e\t%e\t%e\t%e\t%e\t%e\n", (x_low+x_high)/2.0, (double)bins_tr[i],
                 x_low, x_high, y_low, y_high);
      }
      fclose(f_outTrans);
   }

   if (f_outXY != NULL)
   {
      /* Print data */
      for (i=0; i < bins_xy; ++i)
      {
         int j;
         double x_cen = xy_scale*(((double)i+0.5)/(double)bins_xy) - xy_scale/2.0;
         for (j = 0; j < bins_xy; ++j)
         {
            double y_cen = xy_scale*(((double)j+0.5)/(double)bins_xy) - xy_scale/2.0;
            fprintf(f_outXY, "%e\t%e\t%e\n", x_cen, y_cen, (double)bins_X_Y[i][j]);
         }
      }
      fclose(f_outXY);
   }

   printf("Hits: %d\n", count);

   double ratio = (double)count/(double)total;
   double ratio_err = sqrt((double)count)/(double)total;
   printf("Ratio:             %e +- %e\n", ratio, ratio_err);

   printf("Rate in world:     %e Hz\n", rate_w);
   printf("Rate in detector : %e Hz +- %e Hz\n", rate_w*flux_scale*ratio, rate_w*flux_scale*ratio_err);
}
