
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

    int problemSize = atoi(argv[1]);
    double executeTime;
    struct timeval timeStart, timeEnd;
    matrix A, B, C;

    gettimeofday(&timeStart, 0);

    A = getRandom2DArray(problemSize);
    B = getRandom2DArray(problemSize);
    C = matrixProduct(A,B);

    //print2DArray(A);
    //print2DArray(B);
    //print2DArray(C); 

    gettimeofday(&timeEnd, 0);

    executeTime = getExecuteTime(timeStart, timeEnd);  

    saveResultReportOnFile("result_report-serie-runtime.txt", executeTime);

    return 0;
}