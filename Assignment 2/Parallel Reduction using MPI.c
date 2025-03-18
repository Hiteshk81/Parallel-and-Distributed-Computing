#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size;
    int local_value, global_sum;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(rank + 1);
    local_value = rand() % 100; // Each process gets a random value

    printf("Process %d has value %d\n", rank, local_value);

    // Perform parallel reduction to calculate the sum of all values
    MPI_Reduce(&local_value, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Global Sum: %d\n", global_sum);
    }

    MPI_Finalize();
    return 0;
} 
