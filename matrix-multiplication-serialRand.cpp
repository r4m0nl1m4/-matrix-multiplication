
//Main for "matrix-multiplication-serial" C application
//Created by r4m0nl1m4 16/11/2020

//library(ies)
#include <iostream>
#include <stdlib.h>
#include <vector>

//new library(ies)
#include "./matrix.h"
#include "./report.h"

using namespace std;
using matrix = vector<vector<long unsigned int>>;

int main(int argc, char** argv){

    /* Allocate serie environment variables */
    int problemSize;
    double executeTime;
    struct timeval timeStart, timeEnd;
    
    /* Set serie environment variables */
    problemSize = atoi(argv[1]);

    matrix A(problemSize, std::vector<long unsigned int>(problemSize));
    matrix B(problemSize, std::vector<long unsigned int>(problemSize));
    matrix C(problemSize, std::vector<long unsigned int>(problemSize));

    gettimeofday(&timeStart, 0);

    A = get2DArray(problemSize);
    B = get2DArray(problemSize);
    C = matrixProductRandLine(A,B);

    print2DArray(A);
    print2DArray(B);
    print2DArray(C); 

    gettimeofday(&timeEnd, 0);

    executeTime = getExecuteTime(timeStart, timeEnd);  

    saveResultReportOnFile("result_report-serieRand-runtime.txt", executeTime);

    return 0;
}