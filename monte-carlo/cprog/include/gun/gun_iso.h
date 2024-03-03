#ifndef GUN_ISO_H_
#define GUN_ISO_H_

#include "gun/gun.h"

typedef struct {
   double theta;
   double phi;
} iso_output_t;

typedef struct {
   double cos_theta;
   double phi;
} iso_output_c;

typedef union {
   double       pars[2];
   iso_output_t out_t;
   iso_output_c out_c;
} iso_event;

extern gun_ctx gun_iso_init(int use_cos);

#endif /* GUN_ISO_H_ */
