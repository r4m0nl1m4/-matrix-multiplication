
//Main for "matrix" C++ application
//Created by r4m0nl1m4 16/11/2020

//Guard
#ifndef MATRIX_H
#define MATRIX_H

//library(ies)
#include <iostream>
#include <ctime>
#include <vector>

//pre-sets
using namespace std;
using line = std::vector<long unsigned int>;
using matrix = std::vector<line>;

/*
 * Creates Similar Elements Square Matrix
 */
matrix creates2DArray(long unsigned int n){
    matrix A(n, line(n));
    return A;
}

/*
 * Generates Random Similar Elements Matrix
 */
matrix getRandom2DArray(long unsigned int nLines, long unsigned int nColumns){
    matrix A(nLines,line(nColumns));
    long unsigned int number = rand() % 100;
    for (long unsigned int i=0; i<nLines; i++ ){
        for (long unsigned int j=0; j<nColumns; j++ ){
            A[i][j] = number;
        }
    }
    return A;
}

matrix getRandomSquare2DArray(long unsigned int n){
    return getRandom2DArray(n, n);
}

/*
 * Print Similar Elements Matrix
 */
void print2DArray(matrix A){
	long unsigned int nLines = A.size();
    long unsigned int nColumns = A[0].size();
	cout << "\n Matriz " << nLines << "x" << nColumns << endl;
	for (long unsigned int i=0; i<nLines; i++ ){
		for (long unsigned int j=0; j<nColumns; j++ ){
			cout << " " << A[i][j] << " ";
	    }
	    cout << endl; 
    }
}

/*
 * Product of two Similar Elements Square Matrix
 */
matrix matrixProduct(matrix A, matrix B){
	long unsigned int n = A.size();
    matrix C(n,line(n));
    for (long unsigned int i=0; i<A.size(); i++ ){
        for (long unsigned int j=0; j<B.size(); j++ ){
            for (long unsigned int k=0; k<A.size(); k++ )
                C[i][j] += A[i][k] * B[k][j];
        }
    }
    return C;
}

/*
 * Vectors
 */

line getRandVector(long unsigned int n){
    line vetor(n);
    for( long unsigned int i=1; i<=n; i++ )
        vetor[i-1] = i;
    // "Embaralha" vetor sorteando duas posicoes aleatórias e invertendo seus conteúdos por N * 2 vezes
    srand(time(NULL));
    for( long unsigned int i=0; i<(n*2); i++ )
    {
        long unsigned int a = rand() % n;
        long unsigned int b = rand() % n;
        long unsigned int tmp = vetor[a];
        vetor[a] = vetor[b];
        vetor[b] = tmp;
    }
    return vetor;
}

line getZerosVector(long unsigned int n){
    line vetor(n);
    for(long unsigned int i=0; i<n; i++ )
        vetor[i] = 0;
    return vetor;
}

/*
 * Print vector
 */
void printVector(line vetor){
	cout << "\n----Rand Matrix Algorithm \n\n Random lines from matrix A: ";
    for( long unsigned int i = 0; i < vetor.size(); i++ ) cout << vetor[i] << " ";
    cout << std::endl;
}

/*
 * Product of two Similar Elements Square Matrix using rand line number set
 */
matrix matrixProductRandLine(matrix A, matrix B){
    matrix C = creates2DArray(A.size());
    line vec = getRandVector(A.size());
    int l;
	//printVector(vec);
    for (long unsigned int i=0; i<A.size(); i++ ){
    	l = vec[i]-1;
        for (long unsigned int j=0; j<B.size(); j++ ){        	
            for (long unsigned int k=0; k<A.size(); k++ ){
                C[l][j] += A[l][k] * B[k][j];
            }
        }
    }
    return C;
}

#endif