#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

static long num_steps = 100000;
double step;

int main(int argc, char* argv[]) {
    int rank, size, i;
    double x, pi, sum = 0.0, local_sum = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    step = 1.0 / (double)num_steps;

    // Broadcast num_steps to all processes
    MPI_Bcast(&num_steps, 1, MPI_LONG, 0, MPI_COMM_WORLD);

    for (i = rank; i < num_steps; i += size) {
        x = (i + 0.5) * step;
        local_sum += 4.0 / (1.0 + x * x);
    }

    // Reduce all partial sums to calculate the final sum
    MPI_Reduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        pi = step * sum;
        printf("Calculated value of Pi: %lf\n", pi);
    }

    MPI_Finalize();
    return 0;
}
