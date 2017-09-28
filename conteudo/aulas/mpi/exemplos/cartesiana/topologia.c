#include <stdio.h>
#include <mpi.h>

#define TAMANHO 16

#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3

int main(int argc, char *argv[])  {
  
  int numtasks, rank, source, dest, outbuf, i, tag=1;
  int inbuf[4]={MPI_PROC_NULL,MPI_PROC_NULL,MPI_PROC_NULL,MPI_PROC_NULL,};
  int nbrs[4], dims[2]={4,4}, periods[2]={0,0}, reorder=0, coords[2];
  

  MPI_Request reqs[8];
  MPI_Status stats[8];
  MPI_Comm cartcomm;
  
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
      
  if (numtasks == TAMANHO) {

    //criar o plano cartesiano na forma do comunicador cartcomm (saída)
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &cartcomm);
    //MPI_COMM_WORLD: comunicador
    //2: quantidade de dimensões
    //dims: vetor de inteiros com o número de p em cada dimensão
    //periods: vetor de lógicos informando se cada dimensão é periódica ou não
    //reorder: lógico informando se o ranqueamento deve ser re-ordenado ou não

    //obter o rank deste processo dentro do plano cartesiano
    MPI_Comm_rank(cartcomm, &rank);
    
    //obter as coordenadas deste processo dentro do plano cartesiano
    MPI_Cart_coords(cartcomm, rank, 2, coords);

    //obter os ranks dos viznhos acima e abaixo
    MPI_Cart_shift(cartcomm, 0, 1, &nbrs[UP], &nbrs[DOWN]);

    //obter os ranks dos vizinhos à esquerda e à direita
    MPI_Cart_shift(cartcomm, 1, 1, &nbrs[LEFT], &nbrs[RIGHT]);

    printf("rank= %d coords= %d %d  neighbors(u,d,l,r)= %d %d %d %d\n",
	   rank,coords[0],coords[1],nbrs[UP],nbrs[DOWN],nbrs[LEFT],
	   nbrs[RIGHT]);

    outbuf = rank;

    //comunicação assíncrona com os vizinhos
    for (i=0; i<4; i++) {
      dest = nbrs[i];
      source = nbrs[i];
      MPI_Isend(&outbuf, 1, MPI_INT, dest, tag, 
		MPI_COMM_WORLD, &reqs[i]);
      MPI_Irecv(&inbuf[i], 1, MPI_INT, source, tag, 
		MPI_COMM_WORLD, &reqs[i+4]);
    }

    MPI_Waitall(8, reqs, stats);
   
    printf("rank= %d                  inbuf(u,d,l,r)= %d %d %d %d\n",
	   rank,inbuf[UP],inbuf[DOWN],inbuf[LEFT],inbuf[RIGHT]);  
  } else {
    printf("Must specify %d processors. Terminating.\n", TAMANHO);
  }
   
  MPI_Finalize();
  return 0;
}
