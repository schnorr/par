
#include <stdio.h>
#include <omp.h>

long fib (int n)
{
  long x, y;
  if (n < 2)
    return n;

#pragma omp parallel
#pragma omp taskgroup
  {
  
#pragma omp task shared(x) firstprivate(n) untied
  x = fib(n - 1);
#pragma omp task shared(y) firstprivate(n) untied
  y = fib(n - 2);

  }
  return x + y;
}

int main(int argc, char **argv) {
  int n = argc == 2 ? atoi(argv[1]) : 20;
  long res = 0;
  double t0, t1, tdelta;

  
  t0 = omp_get_wtime();
#pragma omp parallel

#pragma omp master
  printf("nts = %d\n", omp_get_num_threads());

#pragma omp single
  {
    res = fib(n);
  }
  t1 = omp_get_wtime();
  tdelta = t1 - t0;
  printf("Fibonacci de %d eh %ld em tempo %0.4f segundos.\n", n, res, tdelta);
  
  return 0;
}

