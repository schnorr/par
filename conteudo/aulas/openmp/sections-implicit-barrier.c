#include <stdio.h>
#include <omp.h>

int main() {
  int a, b, c, d;
  #pragma omp parallel sections
  {
    #pragma omp section
    {
      a = 0;
    }
    
    #pragma omp section
    {
      b = 0;
    }
    #pragma omp section
    {
      c = 0;
    }
    #pragma omp section
    {
      d = 0;
      int i;
      for (i = 0; i < 999999999; i++);
    }
  }
  printf ("Everything has been terminated now.\n");
}
