// omp_critical.cpp
// compile with: /openmp 
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 100000

int main() 
{
  int i;
  int max;
  int a[SIZE];

  for (i = 0; i < SIZE; i++) 
  {
    a[i] = rand();
    printf("%d\n", a[i]);
  }

  max = a[0];
  #pragma omp parallel for num_threads(4)
  for (i = 1; i < SIZE; i++) 
  {
    if (a[i] > max)
    {
      #pragma omp critical
      {
	// compare a[i] and max again because max 
	// could have been changed by another thread after 
	// the comparison outside the critical section
	if (a[i] > max)
	  max = a[i];
      }
    }
  }
   
  printf("max = %d\n", max);
}
