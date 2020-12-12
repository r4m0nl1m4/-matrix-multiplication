
//Main for "matrix-multiplication-serial" C++ application
//Created by r4m0nl1m4 16/11/2020

#include "./matrix.h"
#include "./report.h"

int main(int argc, char** argv){

    int problemSize = atoi(argv[1]);
    double executeTime;
    struct timeval timeStart, timeEnd;
    matrix A, B, C;

    gettimeofday(&timeStart, 0);

    A = getRandomSquare2DArray(problemSize);
    B = getRandomSquare2DArray(problemSize);
    C = matrixProductRandLine(A,B);

    gettimeofday(&timeEnd, 0);

    executeTime = getExecuteTime(timeStart, timeEnd);  

    saveResultReportOnFile("result_report-serieRand-runtime.txt", executeTime);

    //print2DArray(A);
    //print2DArray(B);
    //print2DArray(C); 

    return 0;
}