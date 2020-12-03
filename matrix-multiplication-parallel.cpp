
//Main for "matrix-multiplication-parallel" C application
//Created by r4m0nl1m4 16/11/2020

//library(ies)
#include <ctime>
#include <iostream>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

//new library(ies)
#include "./matrix.h"
#include "./report.h"

using namespace std;
using line = vector<long unsigned int>;

MPI_Status status;

int main(int argc, char **argv)
{
    int N = atoi(argv[1]), numtasks,taskid,numworkers,source,dest,rows,offset,i,j,l,k;

    double timeStart, timeEnd, executeTime;
    double a[N][N],b[N][N],c[N][N];

    /* Start parallel computing */
    MPI_Init(&argc, &argv);

    // Getting the Start Time
    timeStart = MPI_Wtime();

    /* Getting the current processes (rank) */
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

    /* Getting the number of processes (size) */
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    /* Allocate parallel environment variables */
    bool isMaster;

    numworkers = numtasks-1;

    isMaster = (taskid == 0);
    if (isMaster) {
        /* Creates Matrix A and Matrix B */
        double number1 = rand() % 100;
        double number2 = rand() % 100;
        for (int i=0; i<N; i++ ){
            for (int j=0; j<N; j++ ){
                a[i][j] = number1;
                b[i][j] = number2;
            }
        }
        /* send matrix data to the worker tasks */
        rows = N/numworkers;
        offset = 0;
        for (dest=1; dest<=numworkers; dest++){
            MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&a[offset][0], rows*N, MPI_DOUBLE,dest,1, MPI_COMM_WORLD);
            MPI_Send(&b, N*N, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
            offset = offset + rows;
        }
        /* wait for results from all worker tasks */
        for (i=1; i<=numworkers; i++){
            source = i;
            MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&c[offset][0], rows*N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
        }

        // Getting the End Time
        timeEnd = MPI_Wtime();

        // Getting the Time interval
        executeTime = timeEnd-timeStart;
        /*
        cout << endl << " Matrix A: " << endl << endl;
        for ( i=0; i<N; i++ ){
          cout << " ";
          for ( j=0; j<N; j++ ) cout << a[i][j] << " ";
          cout << endl;
        }
        cout << endl;

        cout << endl << " Matrix B: " << endl << endl;
        for ( i=0; i<N; i++ ){
          cout << " ";
          for ( j=0; j<N; j++ ) cout << b[i][j] << " ";
          cout << endl;
        }
        cout << endl;

        cout << endl << " Parallel Product Result Matrix: " << endl << endl;
        for ( i=0; i<N; i++ ){
          cout << " ";
          for ( j=0; j<N; j++ ) cout << c[i][j] << " ";
          cout << endl;
        }
        cout << endl;
        */
        saveResultReportOnFile("result_report-parallel-runtime.txt", executeTime);

        return 0;
    }
    else { /* Send  local to process 0 */
        source = 0;
        MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&a, rows*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, N*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);

        // Matrix Product
        line vec = getRandVector(N);
        //printVector(vec);
        for (i=0; i<N; i++ ){
            l = vec[i]-1;
            for (j=0; j<rows; j++ ){                
                for (k=0; k<N; k++ ){
                    c[l][j] += a[l][k] * b[k][j];
                }
            }
        }
        MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&c, rows*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}