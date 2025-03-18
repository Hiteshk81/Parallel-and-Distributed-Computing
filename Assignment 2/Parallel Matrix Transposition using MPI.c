#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 8

void printMatrix(double matrix[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    int rank, size;
    double matrix[N][N], transposed[N][N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows_per_process = N / size;
    double local_matrix[rows_per_process][N];
    double local_transposed[N][rows_per_process];

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                matrix[i][j] = rand() % 100;
            }
        }
        printf("Original Matrix:\n");
        printMatrix(matrix);
    }

    MPI_Scatter(matrix, rows_per_process * N, MPI_DOUBLE, local_matrix, rows_per_process * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < rows_per_process; i++) {
        for (int j = 0; j < N; j++) {
            local_transposed[j][i] = local_matrix[i][j];
        }
    }

    MPI_Gather(local_transposed, rows_per_process * N, MPI_DOUBLE, transposed, rows_per_process * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Transposed Matrix:\n");
        printMatrix(transposed);
    }

    MPI_Finalize();
    return 0;
} 
S