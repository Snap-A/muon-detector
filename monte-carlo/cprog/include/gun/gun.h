#ifndef GUN_H_
#define GUN_H_

typedef void *gun_ctx;
typedef int (*gun_trans)(double* out);

extern gun_ctx gun_init(int num_params);
extern int gun_config(gun_ctx gt, int idx, gun_trans tr);
extern int gun_event(gun_ctx gt, double *out);
extern void gun_delete(gun_ctx gt);

#endif /* GUN_H_ */
