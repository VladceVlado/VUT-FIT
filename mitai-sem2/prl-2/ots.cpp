// VUT FIT MITAI
// PRL 2019/2020
// Project 2: Odd-even transposition sort algorithm
// Author: Vladimir Dusek
// Login: xdusek27

#include <mpi.h>
#include <iostream>
#include <fstream>

using namespace std;

// For time measurement
#define MEASURE_TIME

const char FILE_NAME[] = "numbers";
const int TAG = 0;

int main(int argc, char *argv[])
{
    int number_of_processes;
    int my_id;
    MPI_Status status;

    // MPI init
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);  // How much processes is running
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);  // Get id of my process

    int my_number;
    int neighbor_number;

    // Master process (with id 0) loads all the values and sends them among the processes
    // Every process will receive 1 value
    if (my_id == 0) {
        fstream file;
        file.open(FILE_NAME, ios::in);

        // Read the whole file
        for (int receiver = 0, number = file.get(); file.good(); receiver++, number = file.get()) {
            // Print loaded number
            cout << number << " ";

            // Send the loaded number to the process with receiver id
            MPI_Send(&number, 1, MPI_INT, receiver, TAG, MPI_COMM_WORLD);
        }

        cout << endl;
        file.close();
    }

    // Every process receives one value
    MPI_Recv(&my_number, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);

    // Calculate the limits for the sorting
    const int odd_limit = 2 * (number_of_processes / 2) - 1;
    const int even_limit = 2 * ((number_of_processes - 1) / 2);

    // Master process measures the time
    #ifdef MEASURE_TIME
        double t1, t2;
        if (my_id == 0) {
            t1 = MPI_Wtime();
        }
    #endif

    // Sorting
    for (int i = 0; i < (number_of_processes + 1) / 2; i++) {

        // Processes with even ID
        if ((!(my_id % 2) || my_id == 0) && (my_id < odd_limit)) {
            // Send my value to my neighbor on the right
            MPI_Send(&my_number, 1, MPI_INT, my_id + 1, TAG, MPI_COMM_WORLD);
            // And receive the lower number
            MPI_Recv(&my_number, 1, MPI_INT, my_id + 1, TAG, MPI_COMM_WORLD, &status);
        }
        // Processes with odd ID
        else if (my_id <= odd_limit) {
            // Receive the value
            MPI_Recv(&neighbor_number, 1, MPI_INT, my_id - 1, TAG, MPI_COMM_WORLD, &status);

            // If received value (the left one) is greater let's do the swap
            if (neighbor_number > my_number) {
                MPI_Send(&my_number, 1, MPI_INT, my_id - 1, TAG, MPI_COMM_WORLD);
                my_number = neighbor_number;
            }
            // If it's not return the same value
            else {
                MPI_Send(&neighbor_number, 1, MPI_INT, my_id - 1, TAG, MPI_COMM_WORLD);
            }
        }
        // Last process can get here
        else {
        }

        // Processes with odd ID
        if ((my_id % 2) && (my_id < even_limit)) {
            // Send my value to my neighbor on the right
            MPI_Send(&my_number, 1, MPI_INT, my_id + 1, TAG, MPI_COMM_WORLD);
            // And receive the lower value
            MPI_Recv(&my_number, 1, MPI_INT, my_id + 1, TAG, MPI_COMM_WORLD, &status);
        }
        // Processes with even ID
        else if (my_id <= even_limit && my_id != 0) {
            // Receive the value
            MPI_Recv(&neighbor_number, 1, MPI_INT, my_id - 1, TAG, MPI_COMM_WORLD, &status);

            // If received value (the left one) is greater let's do the swap
            if (neighbor_number > my_number) {
                MPI_Send(&my_number, 1, MPI_INT, my_id - 1, TAG, MPI_COMM_WORLD);
                my_number = neighbor_number;
            }
            // If it's not return the same value
            else {
                MPI_Send(&neighbor_number, 1, MPI_INT, my_id - 1, TAG, MPI_COMM_WORLD);
            }
        }
        // Last process can get here
        else {
        }
    }

    #ifdef MEASURE_TIME
        if (my_id == 0) {
            t2 = MPI_Wtime();
            cerr << (size_t) ((t2 - t1) * 1000000) << endl;
        }
    #endif

    // Prepare array for the results
    int *results = nullptr;
    if (my_id == 0) {
        results = new int[number_of_processes];
        results[0] = my_number;
    }

    // Distribute the results to the master process (with id 0)
    for (int i = 1; i < number_of_processes; i++) {
        // Master process receives all the values
        if (my_id == 0) {
            MPI_Recv(&neighbor_number, 1, MPI_INT, i, TAG, MPI_COMM_WORLD, &status);
            results[i] = neighbor_number;
        }
        // Process i send his value
        else if (my_id == i) {
            MPI_Send(&my_number, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
        }
        // All the others don't do anything
        else {
        }
    }

    // Master process prints results
    if (my_id == 0) {
        for (int i = 0; i < number_of_processes; i++) {
            cout << results[i] << endl;
        }
    }

    MPI_Finalize();

    return 0;
}
