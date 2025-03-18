#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 65536 // 2^16

void daxpy_serial(double a, double *X, double *Y) {
    for (int i = 0; i < N; i++) {
        X[i] = a * X[i] + Y[i];
    }
}

int main(int argc, char* argv[]) {
    int rank, size;
    double a = 2.5;
    double *X = NULL, *Y = NULL;
    double start_time, end_time, serial_time, parallel_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int elements_per_process = N / size;
    double *local_X = (double*)malloc(elements_per_process * sizeof(double));
    double *local_Y = (double*)malloc(elements_per_process * sizeof(double));

    if (rank == 0) {
        X = (double*)malloc(N * sizeof(double));
        Y = (double*)malloc(N * sizeof(double));
        for (int i = 0; i < N; i++) {
            X[i] = 1.0;
            Y[i] = 2.0;
        }

        // Serial Execution
        start_time = MPI_Wtime();
        daxpy_serial(a, X, Y);
        end_time = MPI_Wtime();
        serial_time = end_time - start_time;

        printf("Serial Execution Time: %lf seconds\n", serial_time);

        // Reset data for parallel execution
        for (int i = 0; i < N; i++) {
            X[i] = 1.0;
            Y[i] = 2.0;
        }
    }

    // Distribute data using MPI_Scatter
    MPI_Scatter(X, elements_per_process, MPI_DOUBLE, local_X, elements_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(Y, elements_per_process, MPI_DOUBLE, local_Y, elements_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    start_time = MPI_Wtime();

    // Perform DAXPY Operation in Parallel
    for (int i = 0; i < elements_per_process; i++) {
        local_X[i] = a * local_X[i] + local_Y[i];
    }

    MPI_Gather(local_X, elements_per_process, MPI_DOUBLE, X, elements_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();
    parallel_time = end_time - start_time;

    if (rank == 0) {
        printf("Parallel Execution Time: %lf seconds\n", parallel_time);
        double speedup = serial_time / parallel_time;
        printf("Speedup: %lf\n", speedup);
        free(X);
        free(Y);
    }

    free(local_X);
    free(local_Y);
    MPI_Finalize();

    return 0;
}
