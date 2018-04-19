#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) 
{ 
  int world_size, rank;
  MPI_Comm everyone;           /* intercommunicator */ 
  char worker_program[100]; 
 
  MPI_Init(&argc, &argv); 
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 
  //choose_worker_program(worker_program); 
  snprintf (worker_program, 100, "./trabalhador");
  MPI_Comm_spawn(worker_program, MPI_ARGV_NULL, 10,  
		 MPI_INFO_NULL, 0, MPI_COMM_SELF, &everyone,  
		 MPI_ERRCODES_IGNORE); 
  /* 
   * Parallel code here. The communicator "everyone" can be used 
   * to communicate with the spawned processes, which have ranks 0,.. 
   * MPI_UNIVERSE_SIZE-1 in the remote group of the intercommunicator 
   * "everyone". 
   */

  /* MPI_Finalize(); */
  /* return 0; */
  
  MPI_Barrier (everyone);
  
  int i;
  for (i = 0; i < 10; i++){
    MPI_Send (&i, 1, MPI_INT, i, 123456, everyone);
  }

  printf ("%d: pai terminando\n", rank);

  MPI_Finalize(); 
  return 0; 
} 
