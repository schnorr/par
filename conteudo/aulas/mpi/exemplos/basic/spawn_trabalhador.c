#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) 
{ 
  int size, rank; 
  MPI_Comm parent; 
  MPI_Init(&argc, &argv); 
  MPI_Comm_get_parent(&parent); 
  if (parent == MPI_COMM_NULL){
    printf ("No parent!\n");
    return -1;
  }
  MPI_Comm_remote_size(parent, &size);
  MPI_Comm_rank(parent, &rank);
   
  if (size != 1) error("Something's wrong with the parent"); 
 
  /* 
   * Parallel code here.  
   * The manager is represented as the process with rank 0 in (the remote 
   * group of) MPI_COMM_PARENT.  If the workers need to communicate among 
   * themselves, they can use MPI_COMM_WORLD. 
   */

  printf ("%d: Oi\n", rank);
  /* MPI_Barrier (parent); */

  /* int buffer; */
  /* MPI_Status status; */
  /* MPI_Recv (&buffer, 1, MPI_INT, 0, 123456, parent, &status); */

  /* printf ("%d: Recebi o dado %d do pai\n", rank, buffer); */

  MPI_Barrier (MPI_COMM_WORLD);
  
  MPI_Finalize(); 
  return 0; 
} 
