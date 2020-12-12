
//Main for "matrix-multiplication-parallel-pthreads.cpp" C++ application
//Created by r4m0nl1m4 16/11/2020 

#include "./matrix.h"
#include <pthread.h>
#include "./report.h"

void *mult(void* arg) 
{ 
  line *data = (line*)arg;
  int k = 0;
  int count = 0;
  int n = data->size()/2;
  int vetor[n];
  int *p = (int*)malloc(sizeof(int));
  for (auto it : *data){
    vetor[count] = it;
    count++;
  }
  for (int i = 0; i < n; i++)
    k = k + vetor[i]*vetor[i+n];
  *p = k;
  pthread_exit(p); 
  return NULL;
} 

line getData(matrix A, matrix B, int l, int c, int problemSize){
  line data = getZerosVector(problemSize*2); 
  for (int k = 0; k < problemSize; k++) 
    data[k] = A[l][k];
  for (int k = 0; k < problemSize; k++) 
    data[k+problemSize] = B[k][c];
  return data;
}

matrix matrixProductByPthreads(matrix A, matrix B){
  int matrixSize = A.size();
  int nElements = matrixSize*matrixSize;
  pthread_t *threads; 

  threads = (pthread_t*)malloc(nElements*sizeof(pthread_t)); 
    
  line data;
  int count = 0; 
  for (int l = 0; l < matrixSize; l++) 
    for (int c = 0; c < matrixSize; c++){
      data = getData(A, B, l, c, matrixSize);
      pthread_create(&threads[count++], NULL, mult, &data);
  } 

  matrix C = getRandomSquare2DArray(matrixSize);
  int l=0, c=0;
  for (int i = 0; i < nElements; i++){ 
    void *k;        
    pthread_join(threads[i], &k);         
    int *p = (int *)k;
    C[l][c] = *p;
    c++;    
    if ((i + 1) % matrixSize == 0){
      l++; 
      c=0;
    }
  }

  return C;
}

int main(int argc, char** argv) 
{
  int problemSize = atoi(argv[1]);
  double executeTime;
  struct timeval timeStart, timeEnd;
  matrix A, B, C;

  gettimeofday(&timeStart, 0);

  A = getRandomSquare2DArray(problemSize);
  B = getRandomSquare2DArray(problemSize);
  C = matrixProductByPthreads(A,B);

  gettimeofday(&timeEnd, 0);

  executeTime = getExecuteTime(timeStart, timeEnd);  

  saveResultReportOnFile("result_report-parallel-pthreads-runtime.txt", executeTime);

  //print2DArray(A);
  //print2DArray(B);
  //print2DArray(C);

  return 0; 
}