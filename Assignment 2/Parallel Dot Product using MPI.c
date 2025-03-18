#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 100

int main(int argc, char* argv[]) {
    int rank, size;
    double local_dot = 0.0, global_dot = 0.0;
    double *A = NULL, *B = NULL;
    double local_A[N / 4], local_B[N / 4];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        A = (double*)malloc(N * sizeof(double));
        B = (double*)malloc(N * sizeof(double));
        for (int i = 0; i < N; i++) {
            A[i] = 1.0; // Example values
            B[i] = 1.0;
        }
    }

    MPI_Scatter(A, N / size, MPI_DOUBLE, local_A, N / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, N / size, MPI_DOUBLE, local_B, N / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < N / size; i++) {
        local_dot += local_A[i] * local_B[i];
    }

    MPI_Reduce(&local_dot, &global_dot, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Dot Product: %lf\n", global_dot);
    }

    if (rank == 0) {
        free(A);
        free(B);
    }

    MPI_Finalize();
    return 0;
} 
