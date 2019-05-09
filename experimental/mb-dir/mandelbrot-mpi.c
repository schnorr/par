#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

/* function prototypes implemented in other files */
double gettime (void);
int compute_mandelbrot_point (double x, double y, int maxiter);
void compute_mandelbrot_coordinates (const double xmin,
				     const double xmax,
				     const double ymin,
				     const double ymax,
				     const int maxiter,
				     const int xres,
				     const int yres,
				     double *xs,   // vector of x coordinates
				     double *ys);  // vector of y coordinates

int main (int argc, char **argv) {
  int NP, rank;
  MPI_Init (&argc, &argv);
  MPI_Comm_size (MPI_COMM_WORLD, &NP);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);

  /* Parse the command line arguments. */
  if (argc != 7) {
    if (rank == 0){
      printf("Usage:   %s <xmin> <xmax> <ymin> <ymax> <maxiter> <xres>\n", argv[0]);
      printf("Example: %s 0.27085 0.27100 0.004640 0.004810 100 1024\n", argv[0]);
    }
    MPI_Finalize();
    return 0;
  }

  /* timing variables */
  double t0, t1;

  /* The window in the plane. */
  const double xmin = atof(argv[1]);
  const double xmax = atof(argv[2]);
  const double ymin = atof(argv[3]);
  const double ymax = atof(argv[4]);

  /* Maximum number of iterations. */
  const int maxiter = atoi(argv[5]);

  /* Image size, width is given, height is computed. */
  const int xres = atoi(argv[6]);
  const int yres = xres; //squared area

  int points = xres*yres;
  if (points % NP != 0){
    printf("Error: number of points (%d) is "
	   "not perfectly divisible by number "
	   "of players (%d)\n",
	   points, NP);
    MPI_Finalize();
    return 0;
  }

  /* Maestro print data read from parameters, for validation purposes */
  if (rank == 0){
    printf("(%f, %f) -> (%f, %f) resolution (%d, %d) maxiter %d\n",
	   xmin, ymin, xmax, ymax, xres, yres, maxiter);
  }

  /* Compute how many points per player */
  int points_per_player = -1;
  if (rank == 0){
    points_per_player = points / NP;
  }
  MPI_Bcast (&points_per_player, 1, MPI_INT, 0, MPI_COMM_WORLD);

  printf("[%d] points_per_player = %d\n", rank, points_per_player);

  printf("Broadcast OKAY\n");

  /* Malloc vectors to receive points from maestro */
  double *xs_player;
  double *ys_player;
  xs_player = malloc (points_per_player * sizeof(double));
  ys_player = malloc (points_per_player * sizeof(double));

  /* Malloc output vector for players */
  int *k_player;
  k_player = malloc (points_per_player * sizeof(int));

  double *xs = NULL, *ys = NULL, *k = NULL;
  if (rank == 0){
    /* Malloc vectors to keep ALL x and y mandelbrot coordinates */
    xs = malloc(xres*yres * sizeof(double));
    ys = malloc(xres*yres * sizeof(double));
    /* Malloc vector to get results from all players */
    k = malloc(xres*yres * sizeof(int));

    compute_mandelbrot_coordinates(xmin, xmax, ymin, ymax, maxiter, xres, yres, xs, ys);
  }

  /* scatter x coordinates among players */
  MPI_Scatter (xs, points_per_player, MPI_DOUBLE, 
	       xs_player, points_per_player, MPI_DOUBLE, 
	       0, MPI_COMM_WORLD);

  /* scatter y coordinates among players */
  MPI_Scatter (ys, points_per_player, MPI_DOUBLE,
	       ys_player, points_per_player, MPI_DOUBLE,
	       0, MPI_COMM_WORLD);

  printf("Scatter OKAY\n");

  /* Computation section (both in maestro and player) */
  t0 = gettime();
  for (int i = 0; i < points_per_player; i++){
    k_player[i] = compute_mandelbrot_point (*xs_player, *ys_player, maxiter);
    xs_player++;
    ys_player++;
  }
  t1 = gettime();

  printf("[%d] Compute OKAY %.6f secs\n", rank, t1-t0);

  MPI_Gather (k_player, points_per_player, MPI_INT,
	      k, points_per_player, MPI_INT,
	      0, MPI_COMM_WORLD);

  printf("Gather OKAY\n");
  
  MPI_Finalize ();
  return 0;
}
