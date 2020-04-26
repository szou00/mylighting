
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"


/*============================================
  IMPORANT NOTE

  Ambient light is represeneted by a color value

  Point light sources are 2D arrays of doubles.
       - The fist index (LOCATION) represents the vector to the light.
       - The second index (COLOR) represents the color.

  Reflection constants (ka, kd, ks) are represened as arrays of
  doubles (red, green, blue)
  ============================================*/


//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
  // i.red =
  color ca = calculate_ambient(alight, areflect);
  color cd = calculate_diffuse(light, dreflect, normal);
  color cs = calculate_specular(light, sreflect, view, normal);

  i.red = ca.red + cd.red + cs.red;
  i.green = ca.green + cd.green + cs.green;
  i.blue = ca.blue + ca.blue + cs.blue;

  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color c;
  c.red = alight.red * areflect[RED];
  c.green = alight.green * areflect[GREEN];
  c.blue = alight.blue * areflect[BLUE];
  return c;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color c;
  normalize(light[LOCATION]);
  normalize(normal);

  double c0 = dot_product(normal,light[LOCATION]);
  if (c0 < 0) {
    c0 = 0;
  }
  c.red = light[COLOR][RED] * dreflect[RED] * c0;
  c.green = light[COLOR][GREEN] * dreflect[GREEN] * c0;
  c.blue = light[COLOR][BLUE] * dreflect[BLUE] * c0;

  return c;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color c;
  normalize(light[LOCATION]);
  normalize(normal);

  double r[3];

  double c0 = dot_product(normal,light[LOCATION]);
  if (c0 < 0) {
    c0 = 0;
  }

  //t = dot_product(normal, l)
  //s = (dot(n*l)) - l

  r[RED] =  2 * normal[RED] * c0 - light[LOCATION][RED];
  r[GREEN] = 2 * normal[GREEN] * c0 - light[LOCATION][GREEN];
  r[BLUE] = 2 * normal[BLUE] * c0 - light[LOCATION][BLUE];

  double ca = dot_product(r,view);
  if (ca < 0) {
    ca = 0;
  }
  double p = pow(ca, 8);

  c.red = light[COLOR][RED] * sreflect[RED] * p;
  c.green = light[COLOR][GREEN] * sreflect[GREEN] * p;
  c.blue = light[COLOR][BLUE] * sreflect[BLUE] * p;

  return c;
}

//limit each component of c to a max of 255
void limit_color( color * c ) {
  if (c->red > 255) {
    c->red = 255;
  }
  if (c->green > 255) {
    c->green = 255;
  }
  if (c->blue > 255) {
    c->blue = 255;
  }
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double magnitude;
  magnitude = sqrt( vector[0] * vector[0] +
                    vector[1] * vector[1] +
                    vector[2] * vector[2] );
  int i;
  for (i=0; i<3; i++) {
    vector[i] = vector[i] / magnitude;
  }
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}


//Calculate the surface normal for the triangle whose first
//point is located at index i in polygons
double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
