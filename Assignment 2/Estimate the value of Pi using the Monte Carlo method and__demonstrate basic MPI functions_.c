#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size, i, count = 0, total_count;
    long long int num_points = 1000000;
    double x, y;
    double start_time, run_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc == 2) {
        num_points = atoll(argv[1]);
    }

    long long int points_per_process = num_points / size;
    srand(rank + 1); // Seed for each process

    start_time = MPI_Wtime();

    for (i = 0; i < points_per_process; i++) {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        if (x*x + y*y <= 1.0) {
            count++;
        }
    }

    MPI_Reduce(&count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi_estimate = 4.0 * total_count / num_points;
        run_time = MPI_Wtime() - start_time;
        printf("Estimated Pi: %lf\n", pi_estimate);
        printf("Execution Time: %lf seconds\n", run_time);
    }

    MPI_Finalize();
    return 0;
}
