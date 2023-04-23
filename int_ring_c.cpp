#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size, N = 10, value = 0;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc > 1) {
        N = std::stoi(argv[1]);
    }

    start_time = MPI_Wtime();

    for (int i = 0; i < N; i++) {
        if (rank == 0) {
            MPI_Send(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&value, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            value += rank;
        } else {
            MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            value += rank;
            MPI_Send(&value, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
        }
    }

    end_time = MPI_Wtime();

    if (rank == 0) {
        std::cout << "total value after " << N << " loops: " << value << std::endl;
        std::cout << "Elapsed time: " << end_time - start_time << " seconds" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
