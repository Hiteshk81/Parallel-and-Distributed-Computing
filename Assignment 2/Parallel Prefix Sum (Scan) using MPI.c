#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 16

int main(int argc, char* argv[]) {
    int rank, size;
    int arr[N], local_value, prefix_sum;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int elements_per_process = N / size;

    if (rank == 0) {
        printf("Original Array: ");
        for (int i = 0; i < N; i++) {
            arr[i] = i + 1;
            printf("%d ", arr[i]);
        }
        printf("\n");
    }

    int *local_data = (int*)malloc(elements_per_process * sizeof(int));
    MPI_Scatter(arr, elements_per_process, MPI_INT, local_data, elements_per_process, MPI_INT, 0, MPI_COMM_WORLD);

    prefix_sum = 0;
    for (int i = 0; i < elements_per_process; i++) {
        local_data[i] += prefix_sum;
        prefix_sum = local_data[i];
    }

    int global_prefix;
    MPI_Scan(&prefix_sum, &global_prefix, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    int correction = global_prefix - prefix_sum;
    for (int i = 0; i < elements_per_process; i++) {
        local_data[i] += correction;
    }

    MPI_Gather(local_data, elements_per_process, MPI_INT, arr, elements_per_process, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Prefix Sum: ");
        for (int i = 0; i < N; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }

    free(local_data);
    MPI_Finalize();
    return 0;
} 