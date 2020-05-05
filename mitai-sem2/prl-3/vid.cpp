// VUT FIT MITAI
// PRL 2019/2020
// Project 3: Viditelnost
// Author: Vladimir Dusek
// Login: xdusek27

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <cmath>
#include <limits>
#include <mpi.h>

// #define MEASURE_TIME
// #define DEBUG

using namespace std;

const int TAG = 0;
const double NEG_INF = numeric_limits<double>::min();
const double NEG_HALF_PI = - M_PI / 2;

int main(int argc, char *argv[])
{
    // Number of the processes
    int num_procs;

    // Process ID of the current process
    int pid;

    // MPI stuff
    MPI_Status status;
    MPI_Comm comm;
    MPI_Info info;

    // MPI init
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs); // How much processes is running
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);       // Get pid of current process

    // Communication split because of the shared memory
    MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0, MPI_INFO_NULL, &comm);

    // Get input length and input length rounded up to the closest power of 2
    int lenght = atoi(argv[2]);
    int length_round = pow(2, ceil(log(lenght) / log(2)));

    // Vector for storing angles computed on input values
    vector<double> angles;

    // Master process parses input string and send numbers to the others
    if (pid == 0) {
        #ifdef DEBUG
            cerr << "--------------------------------------------------------------------------------" << endl;
            cerr << "length: " << argv[2] << endl;
            cerr << "length (rounded): " << length_round << endl;
            cerr << "num_procs: " << num_procs << endl;
            cerr << "--------------------------------------------------------------------------------" << endl;
        #endif

        vector<int> buffer;
        stringstream input_stream(argv[1]);
        string token;

        // Get input to the vector buffer
        while (getline(input_stream, token, ',')) {
            try {
                buffer.push_back(stoi(token));
            }
            catch (invalid_argument const &e) {
                cerr << "Bad input: invalid_argument thrown" << endl;
            }
            catch (out_of_range const &e) {
                cerr << "Integer overflow: out_of_range thrown" << endl;
            }
        }

        // Fill buffer with -1 to the closes power of 2
        for (int i = buffer.size(); i < length_round; i++) {
            buffer.push_back(-1);
        }

        #ifdef DEBUG
            cerr << "buffer: ";
            copy(buffer.begin(), buffer.end(),
            ostream_iterator<int>(cerr, " "));
            cerr << endl;
        #endif

        // Compute my angles and store them
        for (int i = 0; i < length_round / num_procs; i++) {
            int num = buffer[i];
            int idx = pid * (length_round / num_procs) + i;

            // Compute angle
            if (idx == 0) {
                angles.push_back(NEG_HALF_PI);
            }
            else if (num == -1) {
                angles.push_back(NEG_INF);
            }
            else {
                angles.push_back(atan((double)(num - buffer[0]) / (double)idx));
            }
        }

        // Distribute start point to the others
        for (int i = 1; i < num_procs; i++) {
            MPI_Send(&buffer[0], 1, MPI_INT, i, TAG, MPI_COMM_WORLD);
        }

        // Distribute the rest of the numbers to the others
        for (int i = 1; i < num_procs; i++) {
            for (int j = 0; j < length_round / num_procs; j++) {
                int idx = i * (length_round / num_procs) + j;
                MPI_Send(&buffer[idx], 1, MPI_INT, i, TAG, MPI_COMM_WORLD);
            }
        }
    }
    // The others receive numbers from the main and compute their angles
    else {
        // Receive start point
        int start_point;
        MPI_Recv(&start_point, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);

        // Receive numbers
        for (int i = 0; i < length_round / num_procs; i++) {
            int num, idx = pid * (length_round / num_procs) + i;
            MPI_Recv(&num, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);

            // Compute angle
            if (num != -1) {
                angles.push_back(atan((double)(num - start_point) / (double)idx));
            }
            else {
                angles.push_back(NEG_INF);
            }
        }
    }

    // Wait for everyone to get their values and compute angles
    MPI_Barrier(MPI_COMM_WORLD);

    int disp_unit = sizeof(double);
    MPI_Aint size = (length_round / num_procs) * disp_unit;

    // Memory shared_origin - allocate and store original angles
    double *shared_origin = nullptr;
    MPI_Win win_shared_origin;
    MPI_Win_allocate_shared(size, disp_unit, MPI_INFO_NULL, MPI_COMM_WORLD, &shared_origin, &win_shared_origin);
    MPI_Win_shared_query(win_shared_origin, 0, &size, &disp_unit, &shared_origin);

    for (int i = 0; i < length_round / num_procs; i++) {
        int idx = pid * (length_round / num_procs) + i;
        shared_origin[idx] = angles[i];
    }

    // Memory shared - allocate and store angles
    double *shared = nullptr;
    MPI_Win win_shared;
    MPI_Win_allocate_shared(size, disp_unit, MPI_INFO_NULL, MPI_COMM_WORLD, &shared, &win_shared);
    MPI_Win_shared_query(win_shared, 0, &size, &disp_unit, &shared);

    for (int i = 0; i < length_round / num_procs; i++) {
        int idx = pid * (length_round / num_procs) + i;
        shared[idx] = angles[i];
    }

    // Wait for all the allocations
    MPI_Barrier(MPI_COMM_WORLD);

    #ifdef DEBUG
        if (pid == 0) {
            // Print shared_origin
            cerr << "--------------------------------------------------------------------------------" << endl;
            cerr << "BEFORE UP-SWEEP" << endl;
            MPI_Win_shared_query(win_shared_origin, 0, &size, &disp_unit, &shared_origin);
            cerr << "shared_origin:\n    ";
            for (int i = 0; i < length_round - 1; i++) {
                cerr << shared_origin[i] << ",";
            }
            cerr << shared_origin[length_round - 1] << endl;

            // Print shared
            MPI_Win_shared_query(win_shared, 0, &size, &disp_unit, &shared);
            cerr << "shared:\n    ";
            for (int i = 0; i < length_round - 1; i++) {
                cerr << shared[i] << ",";
            }
            cerr << shared[length_round - 1] << endl;
        }

        MPI_Barrier(MPI_COMM_WORLD);
    #endif

    // Master process measures the time
    #ifdef MEASURE_TIME
        double t1, t2;
        if (pid == 0) {
            t1 = MPI_Wtime();
        }
    #endif

    // Prefix sum: up-sweep
    for (int i = 1; i <= log2(length_round); i++) {
        int dst = pow(2, i) - 1 + pid * pow(2, i);

        while (dst < length_round) {
            int src1 = dst - pow(2, i - 1);
            int src2 = dst;

            MPI_Win_shared_query(win_shared, 0, &size, &disp_unit, &shared);
            shared[dst] = max(shared[src1], shared[src2]);

            dst += num_procs * pow(2, i);
        }

        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    #ifdef DEBUG
        if (pid == 0) {
            // Print shared_origin
            cerr << "--------------------------------------------------------------------------------" << endl;
            cerr << "AFTER UP-SWEEP" << endl;
            MPI_Win_shared_query(win_shared_origin, 0, &size, &disp_unit, &shared_origin);
            cerr << "shared_origin:\n    ";
            for (int i = 0; i < length_round - 1; i++) {
                cerr << shared_origin[i] << ",";
            }
            cerr << shared_origin[length_round - 1] << endl;

            // Print shared
            MPI_Win_shared_query(win_shared, 0, &size, &disp_unit, &shared);
            cerr << "shared:\n    ";
            for (int i = 0; i < length_round - 1; i++) {
                cerr << shared[i] << ",";
            }
            cerr << shared[length_round - 1] << endl;
        }

        MPI_Barrier(MPI_COMM_WORLD);
    #endif

    // Prefix sum: clear
    if (pid == 0) {
        MPI_Win_shared_query(win_shared, 0, &size, &disp_unit, &shared);
        shared[length_round - 1] = NEG_HALF_PI;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    #ifdef DEBUG
        if (pid == 0) {
            // Print shared_origin
            cerr << "--------------------------------------------------------------------------------" << endl;
            cerr << "AFTER CLEAR" << endl;
            MPI_Win_shared_query(win_shared_origin, 0, &size, &disp_unit, &shared_origin);
            cerr << "shared_origin:\n    ";
            for (int i = 0; i < length_round - 1; i++) {
                cerr << shared_origin[i] << ",";
            }
            cerr << shared_origin[length_round - 1] << endl;

            // Print shared
            MPI_Win_shared_query(win_shared, 0, &size, &disp_unit, &shared);
            cerr << "shared:\n    ";
            for (int i = 0; i < length_round - 1; i++) {
                cerr << shared[i] << ",";
            }
            cerr << shared[length_round - 1] << endl;
        }

        MPI_Barrier(MPI_COMM_WORLD);
    #endif

    // Prefix sum: down-sweep
    for (int i = log2(length_round); i > 0; i--) {
        int right_idx = pow(2, i) - 1 + pid * pow(2, i);

        while (right_idx < length_round) {
            int left_idx = right_idx - pow(2, i - 1);

            MPI_Win_shared_query(win_shared, 0, &size, &disp_unit, &shared);
            double tmp = shared[left_idx];
            shared[left_idx] = shared[right_idx];
            shared[right_idx] = max(tmp, shared[left_idx]);

            right_idx += num_procs * pow(2, i);
        }

        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    #ifdef MEASURE_TIME
        if (pid == 0) {
            t2 = MPI_Wtime();
            cerr << (size_t) ((t2 - t1) * 1000000) << endl;
        }
    #endif

    #ifdef DEBUG
        if (pid == 0) {
            // Print shared_origin
            cerr << "--------------------------------------------------------------------------------" << endl;
            cerr << "AFTER DOWN-SWEEP" << endl;
            MPI_Win_shared_query(win_shared_origin, 0, &size, &disp_unit, &shared_origin);
            cerr << "shared_origin:\n    ";
            for (int i = 0; i < length_round - 1; i++) {
                cerr << shared_origin[i] << ",";
            }
            cerr << shared_origin[length_round - 1] << endl;

            // Print shared
            MPI_Win_shared_query(win_shared, 0, &size, &disp_unit, &shared);
            cerr << "shared:\n    ";
            for (int i = 0; i < length_round - 1; i++) {
                cerr << shared[i] << ",";
            }
            cerr << shared[length_round - 1] << endl;
            cerr << "--------------------------------------------------------------------------------" << endl;
        }

        MPI_Barrier(MPI_COMM_WORLD);
    #endif

    // Final evaluation and print results
    if (pid == 0) {
        MPI_Win_shared_query(win_shared_origin, 0, &size, &disp_unit, &shared_origin);
        MPI_Win_shared_query(win_shared_origin, 0, &size, &disp_unit, &shared_origin);

        string output = "_";
        for (int i = 1; i < lenght; i++) {
            if (shared_origin[i] > shared[i]) {
                output += ",v";
            }
            else {
                output += ",u";
            }
        }
        output += "\n";
        cout << output << flush;
    }

    MPI_Finalize();

    return 0;
}
