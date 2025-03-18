#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define N 12
#define MAX_ITER 1000
#define EPSILON 1e-5

void initialize(double grid[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == 0 || i == N-1 || j == 0 || j == N-1) {
                grid[i][j] = 100.0; // Boundary condition
            } else {
                grid[i][j] = 0.0;
            }
        }
    }
}

void printGrid(double grid[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f ", grid[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    int rank, size, iter;
    double grid[N][N], new_grid[N][N];
    double diff, max_diff;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    initialize(grid);

    for (iter = 0; iter < MAX_ITER; iter++) {
        diff = 0.0;

        for (int i = 1; i < N-1; i++) {
            for (int j = 1; j < N-1; j++) {
                new_grid[i][j] = 0.25 * (grid[i+1][j] + grid[i-1][j] + grid[i][j+1] + grid[i][j-1]);
                diff = fmax(diff, fabs(new_grid[i][j] - grid[i][j]));
            }
        }

        for (int i = 1; i < N-1; i++) {
            for (int j = 1; j < N-1; j++) {
                grid[i][j] = new_grid[i][j];
            }
        }

        MPI_Allreduce(&diff, &max_diff, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        if (max_diff < EPSILON) break;
    }

    if (rank == 0) {
        printf("Final Heat Distribution:\n");
        printGrid(grid);
    }

    MPI_Finalize();
    return 0;
} 
