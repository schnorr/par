void compute_mandelbrot_coordinates (
  const double xmin,
  const double xmax,
  const double ymin,
  const double ymax,
  const int maxiter,
  const int xres,
  const int yres,
  double *xs, // vector of x coordinates
  double *ys  // vector of y coordinates
  )
{
  /* Precompute pixel width and height. */
  double dx=(xmax-xmin)/xres;
  double dy=(ymax-ymin)/yres;

  /* Coordinates of the current point in the complex plane. */
  double x, y; 
  /* Pixel counters */
  int i,j;

  for (j = 0; j < yres; j++) {
    y = ymax - j * dy;
    for(i = 0; i < xres; i++) {
      x = xmin + i * dx;
      *xs = x;
      xs++;
      *ys = y;
      ys++;
    }
  }
}
