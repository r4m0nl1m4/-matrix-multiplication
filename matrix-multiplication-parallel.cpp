
//Main for "matrix-multiplication-parallel" C++ application
//Created by r4m0nl1m4 16/11/2020

#include "./matrix.h"
#include <mpi.h>
#include "./report.h"

MPI_Status status;

int main(int argc, char **argv)
{
    int N = atoi(argv[1]);
    int numtasks, taskid;
    double timeStart, timeEnd, executeTime;
    int a[N][N],b[N][N],c[N][N];

    /* Start parallel computing */
    MPI_Init(&argc, &argv);

    timeStart = MPI_Wtime();

    /* Gets the current processes (rank) */
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

    /* Gets the number of processes (size) */
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    /* Allocate parallel environment variables */
    bool isMaster;
    int numworkers, source, rows, offset;

    isMaster = (taskid == 0);
    if (isMaster) {
        /* Creates Matrix A and Matrix B */
        int number1 = rand() % 100;
        int number2 = rand() % 100;
        for (int i=0; i<N; i++ ){
            for (int j=0; j<N; j++ ){
                a[i][j] = number1;
                b[i][j] = number2;
            }
        }
        /* send matrix data to the worker tasks */
        numworkers = numtasks-1;
        rows = N/numworkers;
        offset = 0;
        /* send matrix data to the worker tasks */
        for (int dest=1; dest<=numworkers; dest++){
            MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&a[offset][0], rows*N, MPI_INT,dest,1, MPI_COMM_WORLD);
            MPI_Send(&b, N*N, MPI_INT, dest, 1, MPI_COMM_WORLD);
            offset = offset + rows;
        }
        /* wait for results from all worker tasks */
        for (int i=1; i<=numworkers; i++){
            source = i;
            MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&c[offset][0], rows*N, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
        }

        timeEnd = MPI_Wtime();

        executeTime = timeEnd-timeStart;
        /*
        // Print results
        cout << endl << " Matrix A: " << endl << endl;
        for (int i=0; i<N; i++ ){
          cout << " ";
          for (int j=0; j<N; j++ ) cout << a[i][j] << " ";
          cout << endl;
        }
        cout << endl;
        cout << endl << " Matrix B: " << endl << endl;
        for (int i=0; i<N; i++ ){
          cout << " ";
          for (int j=0; j<N; j++ ) cout << b[i][j] << " ";
          cout << endl;
        }
        cout << endl;
        cout << endl << " Parallel Product Result Matrix: " << endl << endl;
        for (int i=0; i<N; i++ ){
          cout << " ";
          for (int j=0; j<N; j++ ) cout << c[i][j] << " ";
          cout << endl;
        }
        cout << endl;
        */
        saveResultReportOnFile("result_report-parallel-runtime.txt", executeTime);
    }
    else { /* Send  local to process 0 */
        source = 0;
        MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&a, rows*N, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, N*N, MPI_INT, source, 1, MPI_COMM_WORLD, &status);

        // Matrix Product
        line vec = getRandVector(N);
        int l;
        //printVector(vec);
        for (int i=0; i<N; i++ ){
            l = vec[i]-1;
            for (int j=0; j<rows; j++ ){                
                for (int k=0; k<N; k++ ){
                    c[l][j] += a[l][k] * b[k][j];
                }
            }
        }

        MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&c, rows*N, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}