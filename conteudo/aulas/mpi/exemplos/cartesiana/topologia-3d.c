#include <stdio.h>
#include <strings.h>
#include <mpi.h>

#define TAMANHO 16

#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3
#define FRONT 4
#define BACK  5

typedef struct {
  MPI_Comm comunicador;
  int rank;
  int coordenadas[3];
  int vizinhos[6];
} coord_t;

coord_t obtem_meu_coord (int topologia[])
{
  coord_t ret = {0};
  int periodo[3] = {0,0,0};
  int reorder = 0;

  MPI_Comm_size(MPI_COMM_WORLD, &np);

  if (np != 2*4*5) {
    printf("Problema!\n");
    MPI_Finalize();
    return 1;
  }
  
  MPI_Cart_create(MPI_COMM_WORLD,
		  3,
		  topologia,
		  periodo,
		  reorder,
		  &cartcomm);
  
  //obter o rank deste processo dentro do plano cartesiano
  MPI_Comm_rank(cartcomm, &rank);
    
  //obter as coordenadas deste processo dentro do plano cartesiano
  MPI_Cart_coords(cartcomm, rank, 3, coordenadas);

  //obter os ranks dos viznhos acima e abaixo
  MPI_Cart_shift(cartcomm, 0, 1, &vizinhos[UP], &vizinhos[DOWN]);
  //obter os ranks dos vizinhos à esquerda e à direita
  MPI_Cart_shift(cartcomm, 1, 1, &vizinhos[LEFT], &vizinhos[RIGHT]);
  //obter os ranks dos vizinhos mais ao fundo e menos ao fundo
  MPI_Cart_shift(cartcomm, 2, 1, &vizinhos[FRONT], &vizinhos[BACK]);

  printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
	 rank,
	 coordenadas[0],
	 coordenadas[1],
	 coordenadas[2],
	 vizinhos[UP],
	 vizinhos[DOWN],
	 vizinhos[LEFT],
	 vizinhos[RIGHT],
	 vizinhos[FRONT],
	 vizinhos[BACK]);
  return ret;
}
  

int main(int argc, char *argv[])  {
  int topologia[3] = {2,4,5};
  
  
  MPI_Init(&argc,&argv);
  coord_t coord = obtem_meu_coord(topologia);

  
  
  MPI_Finalize();
  return 0;
}
