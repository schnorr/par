#include <stdio.h>
#include <stdlib.h>
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

  int n = atoi(argv[1]);

  printf ("sou fib %d\n", n);

  if (n < 2){
    MPI_Send (&n, 1, MPI_INT, 0, 123, parent);
  }else{
    //lançar fib (n-1)
    char **parametros;
    parametros = (char**)malloc(sizeof(char*)*2);
    parametros[0] = (char*)malloc(sizeof(char)*100);
    parametros[1] = NULL;
    snprintf (parametros[0], 100, "%d", n-1);
    MPI_Comm filho1;
    
    MPI_Comm_spawn("fib", parametros, 1,  
		   MPI_INFO_NULL, 0, MPI_COMM_SELF, &filho1,  
		   MPI_ERRCODES_IGNORE); 

    //lançar fib (n-2)
    snprintf (parametros[0], 100, "%d", n-2);
    MPI_Comm filho2;
    
    MPI_Comm_spawn("fib", parametros, 1,  
		   MPI_INFO_NULL, 0, MPI_COMM_SELF, &filho2,  
		   MPI_ERRCODES_IGNORE); 


    int resultado_filho_1;
    int resultado_filho_2;
    MPI_Status status;
    MPI_Recv (&resultado_filho_1, 1, MPI_INT, 0, 123, filho1, &status);
    MPI_Recv (&resultado_filho_2, 1, MPI_INT, 0, 123, filho2, &status);

    int resultado = resultado_filho_1 + resultado_filho_2;
    MPI_Send (&resultado, 1, MPI_INT, 0, 123, parent);
  }
    
  MPI_Finalize(); 
  return 0; 
} 
