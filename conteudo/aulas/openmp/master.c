// omp_master.cpp
// compile with: /openmp 
#include <omp.h>
#include <stdio.h>

int main( ) 
{
  int a[5], i;

#pragma omp parallel
  {
    // Perform some computation.
#pragma omp for
    for (i = 0; i < 5; i++)
      a[i] = i * i;

    // Print intermediate results.
#pragma omp master
    for (i = 0; i < 5; i++)
      printf("a[%d] = %d\n", i, a[i]);
      
    // Wait.
#pragma omp barrier

    // Continue with the computation.
#pragma omp for
    for (i = 0; i < 5; i++)
      a[i] += i;
  }
}
