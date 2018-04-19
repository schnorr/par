#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) 
{ 
  int world_size, rank;
  MPI_Comm everyone;           /* intercommunicator */ 
  char worker_program[100]; 
 
  MPI_Init(&argc, &argv); 
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  char **parametros;
  parametros = (char**)malloc(sizeof(char*)*2);
  parametros[0] = (char*)malloc(sizeof(char)*100);
  parametros[1] = NULL;
  snprintf (parametros[0], 100, "%s\n", argv[1]);

  printf ("oi %s\n",parametros[0]);
  
  MPI_Comm_spawn("./fib", parametros, 1,  
		 MPI_INFO_NULL, 0, MPI_COMM_SELF, &everyone,  
		 MPI_ERRCODES_IGNORE); 

  int resultado;
  MPI_Status status;
  MPI_Recv (&resultado, 1, MPI_INT, MPI_ANY_SOURCE, 123, everyone, &status);
  printf ("%d: recebi de %d o resultado %d\n", rank, status.MPI_SOURCE, resultado);
  MPI_Finalize();
  return 0;

} 
