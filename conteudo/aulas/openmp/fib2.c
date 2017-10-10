#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define THRESHOLD 30

long fib (int n)
{
  long x, y;
  if (n < 2)
    return n;

#pragma omp task shared(x) firstprivate(n) final(n <= THRESHOLD)
  x = fib(n-1);

#pragma omp task shared(y) firstprivate(n) final(n <= THRESHOLD)
  y = fib(n-2);

#pragma omp taskwait
  return x+y;
}

int main(int argc, char **argv) {
  int n = argc == 2 ? atoi(argv[1]) : 20;
  long res = 0;

#pragma omp parallel default(none) shared(n) shared(res)
  {
#pragma omp single
  {
    double t0, t1, tdelta;
    t0 = omp_get_wtime();
    res = fib(n);
    t1 = omp_get_wtime();
    tdelta = t1 - t0;
    printf("Fibonacci de %d eh %ld em tempo %0.4f segundos.\n", n, res, tdelta);
  }
  }
  
  return 0;
}

