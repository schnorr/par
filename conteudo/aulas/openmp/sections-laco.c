#include <stdio.h>
#include <omp.h>

#define TAMANHO 9999999

double a[TAMANHO];
double b[TAMANHO];
double c[TAMANHO];
double d[TAMANHO];

int main() {
  int i;
  int n = TAMANHO;
  
  #pragma omp parallel shared(n,a,b,c,d) private(i)
  {
    #pragma omp sections nowait
    {
       #pragma omp section
         for (i=0; i<n; i++)
            d[i] = 1.0/c[i];
       #pragma omp section
         for (i=0; i<n-1; i++)
            b[i] = (a[i] + a[i+1])/2;
    }
  } 
  printf ("Everything has been terminated now.\n");
}
