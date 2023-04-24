// mpic++ -o int_ring_large_array int_ring_large_array.cpp
// mpirun -np 4 ./int_ring_large_array 100
#include <iostream>
#include <mpi.h>
#include <vector>

int main(int argc, char *argv[]) { //variable number, variable name 
    int rank, size, N = 10; //, value = 0; 
    double start_time, end_time;
    const size_t ARRAY_SIZE = (2 * 1024 * 1024) / sizeof(int);
    std::vector<int> value(ARRAY_SIZE, 0); //initiate 2Mbyte array

    MPI_Init(&argc, &argv); // start MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);//MPI_COMM_WORLD communication within the world, rank-rank of processor
    MPI_Comm_size(MPI_COMM_WORLD, &size);//total number of processor 

    if (argc > 1) {
        N = std::stoi(argv[1]); // input argv[1] into N if input number is larger than 1, first is file name 
    }

        // Perform actions specific to Process 0 (rank == 0)
    if (rank == 0) {
        // Initialize the array or perform any other computations
        for (size_t i = 0; i < ARRAY_SIZE; i++) {
            value[i] = i;
        }
    }

    start_time = MPI_Wtime(); //test starttime, Wtime counts real time

    for (int i = 0; i < N; i++) {
        
        if (rank == 0)  {
            MPI_Send(value.data(), ARRAY_SIZE, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
            MPI_Recv(value.data(), ARRAY_SIZE, MPI_INT, (rank + size - 1) % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }else {
            MPI_Recv(value.data(), ARRAY_SIZE, MPI_INT, (rank + size - 1) % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(value.data(), ARRAY_SIZE, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
        }
    }

    end_time = MPI_Wtime(); //stop timing 

    if (rank == 0) {
        double bandwidth=(2.0 * ARRAY_SIZE * sizeof(int)) / (1024 * 1024 * (end_time - start_time)); // In MBytes per second
        double latency = (end_time - start_time) / (2.0 * N * (size - 1)); 
        std::cout << "number of processor: " << size << std::endl;
        std::cout << "number of loop: "  << N << std::endl;
        std::cout << "Elapsed time: " << end_time - start_time << " seconds" << std::endl;
        std::cout << "Bandwidth: " << bandwidth << " MBytes/s" << std::endl;
        std::cout << "Latency: " << latency << " seconds" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
