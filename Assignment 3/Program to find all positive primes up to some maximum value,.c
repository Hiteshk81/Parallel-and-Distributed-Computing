#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int is_prime(int n) {
    if (n < 2) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
}

int main(int argc, char* argv[]) {
    int rank, size, num, max_value = 1000;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        int current_number = 2;
        int result;
        MPI_Status status;

        while (current_number <= max_value || size > 1) {
            MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            if (result > 0) {
                printf("Prime: %d\n", result);
            }
            if (current_number <= max_value) {
                MPI_Send(&current_number, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
                current_number++;
            } else {
                int stop_signal = -1;
                MPI_Send(&stop_signal, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
                size--;
            }
        }
    } else {
        int num_to_test;
        int initial_msg = 0;
        MPI_Send(&initial_msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        while (1) {
            MPI_Recv(&num_to_test, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (num_to_test == -1) break;
            int result = is_prime(num_to_test) ? num_to_test : -num_to_test;
            MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
