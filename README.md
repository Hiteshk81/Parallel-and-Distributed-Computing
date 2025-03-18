MPI Programming Assignment

This repository contains MPI-based C programs that demonstrate various parallel computing concepts using the Message Passing Interface (MPI). The programs include tasks like calculating Pi, performing matrix operations, finding primes, and simulating heat distribution.

Table of Contents

Estimate Pi using Monte Carlo (mpi_monte_carlo.c)

Matrix Multiplication with Time Comparison (mpi_matrix_multiplication.c)

Parallel Sorting using Odd-Even Sort (mpi_parallel_sorting.c)

Heat Distribution Simulation (mpi_heat_distribution.c)

Parallel Reduction (mpi_parallel_reduction.c)

Parallel Dot Product (mpi_parallel_dot_product.c)

Parallel Prefix Sum using MPI_Scan (mpi_parallel_prefix_sum.c)

Parallel Matrix Transposition (mpi_parallel_matrix_transposition.c)

DAXPY Operation and Speedup (mpi_daxpy_loop.c)

Pi Calculation using MPI_Bcast and MPI_Reduce (mpi_calculate_pi.c)

Finding Primes using MPI_Recv and MPI_Send (mpi_prime_finder.c)

Requirements

MPI Library (e.g., OpenMPI)

C Compiler (e.g., gcc)

Installation and Compilation

Install OpenMPI using your package manager:

sudo apt-get install openmpi-bin openmpi-common libopenmpi-dev

Clone the repository:

git clone https://github.com/your-username/mpi-assignments.git
cd mpi-assignments

Compile any program using mpicc:

mpicc mpi_calculate_pi.c -o mpi_calculate_pi

Running Programs

Run using mpirun:

mpirun -np 4 ./mpi_calculate_pi

Replace 4 with the desired number of processes.

Check results and outputs in the terminal.

Contribution

Feel free to fork this repository and contribute by adding new features or fixing bugs. Submit a pull request with detailed explanations of your changes.

Developed by Hitesh
B.Tech COE | TIET
