#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void oddEvenSort(int *arr, int n, int rank, int size) {
    int phase, temp;
    MPI_Status status;

    for (phase = 0; phase < n; phase++) {
        if ((phase % 2 == 0 && rank % 2 == 0) || (phase % 2 != 0 && rank % 2 != 0)) {
            if (rank < size - 1) {
                MPI_Send(&arr[n-1], 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                MPI_Recv(&temp, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &status);
                if (arr[n-1] > temp) {
                    arr[n-1] = temp;
                }
            }
        } else {
            if (rank > 0) {
                MPI_Recv(&temp, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
                MPI_Send(&arr[0], 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
                if (temp > arr[0]) {
                    arr[0] = temp;
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    int rank, size, n = 16;
    int *arr = NULL;
    int local_arr[2];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        arr = (int*)malloc(n * sizeof(int));
        printf("Unsorted Array: ");
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % 100;
            printf("%d ", arr[i]);
        }
        printf("\n");
    }

    MPI_Scatter(arr, 2, MPI_INT, local_arr, 2, MPI_INT, 0, MPI_COMM_WORLD);

    oddEvenSort(local_arr, 2, rank, size);

    MPI_Gather(local_arr, 2, MPI_INT, arr, 2, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Sorted Array: ");
        for (int i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
        free(arr);
    }

    MPI_Finalize();
    return 0;
}
