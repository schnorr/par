#include <stdio.h>
int compute_mandelbrot_point (double x, double y, int maxiter)
{
  int k; /* Iteration counter */
  double u = 0.0;
  double v = 0.0;
  double u2 = u * u;
  double v2 = v * v;
  /* iterate the point */
  for (k = 1; k < maxiter && (u2 + v2 < 4.0); k++) {
    v = 2 * u * v + y;
    u = u2 - v2 + x;
    u2 = u * u;
    v2 = v * v;
  }
  return k;
}
