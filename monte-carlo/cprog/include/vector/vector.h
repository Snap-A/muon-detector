#ifndef VECTOR_H_
#define VECTOR_H_

/* Constants */
extern const int x_c;
extern const int y_c;
extern const int z_c;

/* Types */
typedef double vec3[3];
typedef double mat33[3][3];

extern void copy_vec(vec3 out, const vec3 in);

extern void scale_vec(vec3 v, double s);
extern void scale_vec2(vec3 out, const vec3 in, double s);

extern void diff_vec(vec3 out, const vec3 left, const vec3 right);

extern void add_vec(vec3 out, const vec3 left, const vec3 right);

extern double dot_vec(const vec3 v1, const vec3 v2);

extern void cross_vec(vec3 out, const vec3 a, const vec3 b);

extern void mul_matrix(vec3 x, const mat33 M, const vec3 y);
#endif /* VECTOR_H_ */
