CC = mpicc
CFLAGS = -O3 -g
LDFLAGS = -L$(HOME)/install/akypuera/lib/ -laky -lrastro
OBJ = timing_functions.o \
      compute_mandelbrot_coordinates.o \
      compute_one_mandelbrot_point.o \
      mandelbrot-mpi.o \

%.o:%.c
	$(CC) -c -o $@ $< $(CFLAGS)

mandelbrot-mpi-akypuera: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o
