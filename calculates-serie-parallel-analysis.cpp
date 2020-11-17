
//Main for "calculates-speedup" C application
//Created by r4m0nl1m4 16/11/2020

//library(ies)
#include <stdlib.h>

//new library(ies)
#include "./report.h"

int main(int argc, char** argv){

    saveSpeedupReportOnFile("result_report-speedup.txt", "result_report-serieRand-runtime.txt", "result_report-parallel-runtime.txt");
    saveEfficiencyReportOnFile("result_report-efficiency.txt", "result_report-speedup.txt", "result_report-parallel-cores.txt");

    return 0;
}