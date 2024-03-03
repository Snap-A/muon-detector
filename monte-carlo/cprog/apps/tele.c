/**********************************************************/
/* Run the muon telescope simulation and record the       */
/* ratio of detected events over all generated events.    */
/**********************************************************/

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
   printf("Usage:\n%s [-e <num>] [-h] [-s <double>] [-w <double>] <theta>\n", name);
   printf("\n-- Options:\n");
   printf("-e <num>    : Set the number of events to simulate. (Default is 1,000,000)\n");
   printf("-f <num>    : Set the simulated flux of particle.\n");
   printf("              0 = PDG flux. (~ cos^2 theta)\n");
   printf("              1 = Isotropic flux.\n");
   printf("-h          : Print this help text.\n");
   printf("-l <double> : Set the (longer) length of the detectors [m]. (Default is 0.1 m)\n");
   printf("-s <double> : Set the separation between detectors [m]. (Default is 1.0 m)\n");
   printf("-t <path>   : Change logic to record 'hit' in give file as theta,phi. (Default is not to do that)\n");
   printf("-u          : Disable 'foreshortening' rule on particles in first detector. (Default is to use it)\n");
   printf("-w <double> : Set the (shorter) width of the detectors [m]. (Default is 0.1 m)\n");
   printf("\n-- Positional arguments:\n");
   printf("<theta>          : Angle to zenith [radians].\n");
}

/* Main */
int main(int argc, char *argv[])
{
   int    i, b;
   FILE   *f_outR = NULL;
   FILE   *f_outH = NULL;
   double theta_d = 0.0;
   int    flux = 0;
   double total_rate_per_m2 = mu_pdg_i * pi / 2.0; /* Hz/m^2 */

   int index, type;
   int c;

   int    total = 1000000;
   double separation = 1.0;
   double length = 0.1;
   double width = 0.1;
   int    use_f = 1;

   opterr = 0;
   while ((c = getopt (argc, argv, "e:f:hl:s:t:uw:")) != -1)
      switch (c)
      {
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
      case 's':
         separation = strtod(optarg, NULL);
         break;
      case 't':
         f_outH = fopen(optarg, "w");
         break;
      case 'u':
         use_f = 0;
         break;
      case 'w':
         width = strtod(optarg, NULL);
         break;
      case '?':
         if (strchr("e", optopt) != 0)
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

   /* Geometry descripton of the telesope */
   g_rectangle rectangle;

   vec3 r_e1, r_e2;
   double l_e1, l_e2;
   gun_ctx contextI = NULL;
   gun_ctx contextL = NULL;
   gun_ctx contextW = NULL;

   /* Scaling factor due to flux model. Default is 1.0 */
   double  flux_scale = 1.0;
   double  rate_det1;

   if (flux == 0)
   {
      /* Set PDFs for particle gun */
      /* Return theta = 0, cos(theta) = 1 */
      contextI = gun_pdg_init(0);
   }
   if (flux == 1)
   {
      /* Set PDFs for particle gun */
      /* Return theta = 0, cos(theta) = 1 */
      contextI = gun_iso_init(0);
      flux_scale = 1.0;
   }

   if (f_outH == NULL)
   {
      /* Return coordinate on detector 1: -length/2.0 <-> length/2.0 */
      contextL = gun_range_init(-length/2.0, length/2.0);
      /* Return coordinate on detector 1: -width/2.0 <-> width/2.0 */
      contextW = gun_range_init(-width/2.0, width/2.0);
   }
   else
   {
      /* Restrict origin to smaller area */
      /* Return coordinate on detector 1: -length/2000.0 <-> length/2000.0 */
      contextL = gun_range_init(-length/2000.0, length/2000.0);
      /* Return coordinate on detector 1: -width/2000.0 <-> width/2000.0 */
      contextW = gun_range_init(-width/2000.0, width/2000.0);
   }
   
   /* Hit counter */
   int    count = 0;

   /* Rotational axis when point the telescope -> x-axis */
   vec3 rot_axis = { 1.0, 0.0, 0.0 };
   
   /* Set fixed values for rectangle (2nd detector) */
   rectangle.origin[x_c] = 0.0;
   rectangle.origin[y_c] = 0.0;
   rectangle.origin[z_c] = -separation/2.0;

   rectangle.edge1[x_c] = 1.0;
   rectangle.edge1[y_c] = 0.0;
   rectangle.edge1[z_c] = 0.0;
   rectangle.edge1_len = length/2.0;
   
   rectangle.edge2[x_c] = 0.0;
   rectangle.edge2[y_c] = 1.0;
   rectangle.edge2[z_c] = 0.0;
   rectangle.edge2_len = width/2.0;

   /* Rotate telescope detector 2 */
   if (fabs(theta_d) > 1E-10)
   {
      vec3 out;
      
      rotate_vec(out, rectangle.origin, rot_axis, theta_d);
      copy_vec(rectangle.origin, out);
      rotate_vec(out, rectangle.edge1, rot_axis, theta_d);
      copy_vec(rectangle.edge1, out);
      rotate_vec(out, rectangle.edge2, rot_axis, theta_d);
      copy_vec(rectangle.edge2, out);
   }

   /* Find normal for detector 2 */
   cross_vec(rectangle.normal, rectangle.edge1, rectangle.edge2);

   for (i=0; i < total; ++i)
   {
      int        hit, retVal;
      double     trans, l1, l2;
      double     x_0, y_0;
      iso_event  evt_i;
      pdg_event  evt_p;
      double     t, p;

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

      /* Set particle values */
      part.origin[x_c] = x_0;
      part.origin[y_c] = y_0;
      part.origin[z_c] = separation/2.0;

      /* Rotate telescope detector 1 */
      if (fabs(theta_d) > 1E-10)
      {
         vec3 out;
      
         rotate_vec(out, part.origin, rot_axis, theta_d);
         copy_vec(part.origin, out);
      }

      /* Set particle direction */
      part.direction[x_c] = sin(t)*cos(p);
      part.direction[y_c] = sin(t)*sin(p);
      part.direction[z_c] = cos(t);

      if (use_f)
      {
         /* Obtain dot product to normal to enforce foreshortening effect */
         double f_size = fabs(dot_vec(rectangle.normal, part.direction));

         if (f_size < drand48())
         {
            /* Address over-density of angled particles due to foreshortening by rejecting this event */
            --i;
            continue;
         }
      }
      
      retVal = intersect_rect(part, rectangle,
                              &trans, &hit, &l1, &l2);
      if ((0 == retVal)&&(1 == hit))
      {
         if (f_outH != NULL)
         {
            fprintf(f_outH, "%e \t %e\n", t, p);
         }
         ++count;
      }
   }

   gun_delete(contextI);
   gun_delete(contextL);
   gun_delete(contextW);

   printf("Hits: %d\n", count);

   double ratio = (double)count/(double)total;
   double ratio_err = sqrt((double)count)/(double)total;
   printf("Ratio: %e +- %e\n", ratio, ratio_err);

   /* Scale for total flux through detector 1 */
   if (flux == 0)
   {
      rate_det1 = r_tot_PDG(theta_d, width*length);
   }
   else if (flux == 1)
   {
      rate_det1 = total_rate_per_m2 * width * length;
   }

   printf("Rate in detector 1: %e Hz\n", rate_det1);
   printf("Rate in telescope : %e Hz +- %e Hz\n", rate_det1*flux_scale*ratio, rate_det1*flux_scale*ratio_err);

   if (f_outR != NULL)
      fclose(f_outR);
   
   return 0;
}
