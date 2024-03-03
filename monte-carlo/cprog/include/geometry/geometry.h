#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include "vector/vector.h"

/* Defines a 'line' in three dimensions */
/**
 ** 'origin'    : A point on the line
 ** 'direction' : The direction vector along the line. Can point in any of the two
 **                possible alignment with the points on the line.
 **                *MUST* be a unit vector (length == 1.0)!
 **/
typedef struct g_line {
   vec3 origin;
   vec3 direction;
} g_line;

/* Defines a 'plane' (with infinite area) in three dimensions */
/**
 ** 'origin' : A point on the plane
 ** 'normal' : The vector normal to the plane. Can point in any of the two
 **                possible directions.
 **                *MUST* be a unit vector (length == 1.0)!
 **/
typedef struct g_plane {
   vec3 origin;
   vec3 normal;
} g_plane;

/* Defines a 'rectangle' in three dimensions */
/**
 ** 'origin'    : The center point of the rectangle (where the two diagonals cross).
 ** 'normal'    : The vector normal to the rectangle. Can point in any of the two
 **                 possible directions.
 **                 *MUST* be a unit vector (length == 1.0)!
 ** 'edgeN'     : The N'th coordinate unit vector inside the plane of the rectangle.
 **                 Two edges must be provided, having a dot-product value of zero with 'normal'
 **                 and with each other.
 ** 'edgeN_len' : The length of the side described by 'edgeN'. It is measured from the origin
 **                 point, and therefore half the value of the total side of the rectangle.
 **/
typedef struct g_rectangle {
   vec3 origin;
   vec3 normal;
   vec3 edge1;
   vec3 edge2;
   double edge1_len;
   double edge2_len;
} g_rectangle;

/* Defines a 'box' in three dimensions */
/**
 ** 'origin'    : The center point of the box (where the four diagonals cross).
 ** 'edgeN'     : The N'th coordinate unit vectors describing the surfaces of the box.
 **                 Three edges must be provided, having a dot-product value of zero
 **                 with each other.
 ** 'edgeN_len' : The length of the side described by 'edgeN'. It is measured from the origin
 **                 point, and therefore half the value of the total side of the box.
 **/
typedef struct g_box {
   vec3 origin;
   vec3 edge1;
   vec3 edge2;
   vec3 edge3;
   double edge1_len;
   double edge2_len;
   double edge3_len;
} g_box;

extern
void rotate_vec(vec3 v_out, const vec3 v_in,
                const vec3 rot_axis, double phi);

extern
int intersect_plane(const g_line line,
                    const g_plane plane,
                    double *translation, vec3 cross_point);

extern
int intersect_rect(const g_line line,
                   const g_rectangle rectangle,
                   double *translation, int *hit,
                   double *local_1, double *local_2);

extern
int intersect_box(const g_line line,
                  const g_box box,
                  double translation[2], int *hit,
                  double local_1[2], double local_2[2], double local_3[2]);

#endif /* GEOMETRY_H_ */
