#ifndef GUN_PDG_H_
#define GUN_PDG_H_

#include "gun/gun.h"

typedef struct {
   double theta;
   double phi;
} pdg_output_t;

typedef struct {
   double cos_theta;
   double phi;
} pdg_output_c;

typedef union {
   double       pars[2];
   pdg_output_t out_t;
   pdg_output_c out_c;
} pdg_event;

extern gun_ctx gun_pdg_init(int use_cos);

#endif /* GUN_PDG_H_ */
