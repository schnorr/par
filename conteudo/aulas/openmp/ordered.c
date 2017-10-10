#include <stdio.h>
#include <omp.h>

int main( ) 
{
    int i;
    #pragma omp parallel for
    for (i = 0 ; i < 5 ; i++)
      printf("iteration %d\n", i);
}
