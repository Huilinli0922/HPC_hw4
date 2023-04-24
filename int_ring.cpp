// mpic++ -o int_ring int_ring.cpp
// mpirun -np 4 ./int_ring 100
#include <iostream>
#include <mpi.h>


int main(int argc, char *argv[]) { //variable number, variable name 
    int rank, size, N = 10, value = 0; 
    double start_time, end_time;


    MPI_Init(&argc, &argv); // start MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);//MPI_COMM_WORLD communication within the world, rank-rank of processor
    MPI_Comm_size(MPI_COMM_WORLD, &size);//total number of processor 


    if (argc > 1) {
        N = std::stoi(argv[1]); // input argv[1] into N if input number is larger than 1, first is file name 
    }

    start_time = MPI_Wtime(); //test starttime, Wtime counts real time

    for (int i = 0; i < N; i++) {
        if (rank == 0) {
            MPI_Send(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD); //send value length 1 
            MPI_Recv(&value, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);//size-1 is process nnumber
            value += rank;
        } else {
            MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            value += rank;
            MPI_Send(&value, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
        }
    }

    end_time = MPI_Wtime(); //stop timing 

    if (rank == 0) {
        double bandwidth=(2.0 * sizeof(int)) / (1024 * 1024 * (end_time - start_time)); // In MBytes per second
        double latency = (end_time - start_time) / (2.0 * N * (size - 1)); 
        std::cout << "number of processor: " << size << std::endl;
        std::cout << "number of loop: "  << N << std::endl;
        std::cout << "total sum: " << value  << std::endl;
        std::cout << "Elapsed time: " << end_time - start_time << " seconds" << std::endl;
        std::cout << "Bandwidth: " << bandwidth << " MBytes/s" << std::endl;
        std::cout << "Latency: " << latency << " seconds" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
