#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>
#include <assert.h>

using namespace std;

#define TIME_NOW std::chrono::high_resolution_clock::now()
#define TIME_DIFF(gran, start, end) std::chrono::duration_cast<gran>(end - start).count()

//#include "./header/single_thread.h"
#include "./header/multi_thread.h"

void singleThread(int N, int *matA, int *matB, int *output)
{
  //assert( N>=4 and N == ( N &~ (N-1)));
  for(int rowA = 0; rowA < N; rowA +=2) {
    for(int colB = 0; colB < N; colB += 2){
      int sum = 0;
      for(int iter = 0; iter < N; iter++) 
      {
        sum += matA[rowA * N + iter] * matB[iter * N + colB];
        sum += matA[(rowA+1) * N + iter] * matB[iter * N + colB];
        sum += matA[rowA * N + iter] * matB[iter * N + (colB+1)];
        sum += matA[(rowA+1) * N + iter] * matB[iter * N + (colB+1)];
      }

      // compute output indices
      int rowC = rowA>>1;
      int colC = colB>>1;
      int indexC = rowC * (N>>1) + colC;
      output[indexC] = sum;
    }
  }
}

void singleThreadB_ijk(int N, int *matA, int *matB, int *output)
{
    int blockSize = 2;
  //assert( N>=4 and N == ( N &~ (N-1)));
  for(int rowA=0; rowA<N; rowA +=blockSize)
    {
        for(int colB=0; colB<N; colB = colB + blockSize)
        {
            int sum = 0;
            for (int k=0; k < N; k = k + blockSize)
            {
                for(int ii=rowA; ii<rowA+blockSize; ii++)
                {
                    for(int jj=colB; jj < colB+blockSize; jj++)
                    {
                        for(int kk=k; kk < k+blockSize; kk++)
                        {
                            sum += matA[ii*N + kk] * matB[kk*N+jj];
                        }
                    }
                }
            }
      // compute output indices
      int rowC = rowA>>1;
      int colC = colB>>1;
      int indexC = rowC * (N>>1) + colC;
      output[indexC] = sum;
        }
  }
}

void singleThreadB_ikj(int N, int *matA, int *matB, int *output)
{
    int blockSize = 2;
  //assert( N>=4 and N == ( N &~ (N-1)));
  for(int rowA=0; rowA<N; rowA +=blockSize)
    {
        for (int k=0; k < N; k = k + blockSize)
        {
            int sum = 0;
            for(int colB=0; colB<N; colB = colB + blockSize)
            {
                for(int ii=rowA; ii<rowA+blockSize; ii++)
                {
                    for(int jj=colB; jj < colB+blockSize; jj++)
                    {
                        for(int kk=k; kk < k+blockSize; kk++)
                        {
                            sum += matA[ii*N + kk] * matB[kk*N+jj];
                        }
                    }
                }
                // compute output indices
                int rowC = rowA>>1;
                int colC = colB>>1;
                int indexC = rowC * (N>>1) + colC;
                output[indexC] = sum;
            }
        }
    }
}

void singleThread_kij(int N, int *matA, int *matB, int *output)
{
  for(int i = 0; i < ((N>>1)*(N>>1)); ++i)
      output[i] = 0;
  for(int iter = 0; iter < N; iter ++) {
    for(int rowA = 0; rowA < N; rowA +=2){
      for(int colB = 0; colB < N; colB+=2) 
      {
        int sum = 0;
        sum += matA[rowA * N + iter] * matB[iter * N + colB];
        sum += matA[(rowA+1) * N + iter] * matB[iter * N + colB];
        sum += matA[rowA * N + iter] * matB[iter * N + (colB+1)];
        sum += matA[(rowA+1) * N + iter] * matB[iter * N + (colB+1)];
       
        //compute output indices
        int rowC = rowA>>1;
        int colC = colB>>1;
        int indexC = rowC * (N>>1) + colC;
        output[indexC] += sum;
      }
    }
  }
}

void singleThread_ikj(int N, int *matA, int *matB, int *output)
{
  for(int i = 0; i < ((N>>1)*(N>>1)); ++i)
      output[i] = 0;
  for(int rowA = 0; rowA < N; rowA += 2) {
    for(int iter = 0; iter < N; iter ++){
      for(int colB = 0; colB < N; colB+=2) 
      {
        int sum = 0;
        sum += matA[rowA * N + iter] * matB[iter * N + colB];
        sum += matA[(rowA+1) * N + iter] * matB[iter * N + colB];
        sum += matA[rowA * N + iter] * matB[iter * N + (colB+1)];
        sum += matA[(rowA+1) * N + iter] * matB[iter * N + (colB+1)];
       
        //compute output indices
        int rowC = rowA>>1;
        int colC = colB>>1;
        int indexC = rowC * (N>>1) + colC;
        output[indexC] += sum;
      }
    }
  }
  
}
// Used to cross-check answer. DO NOT MODIFY!
void reference(int N, int *matA, int *matB, int *output)
{
  // enforce N to be power of 2 and greater than 2
  //assert( N>=4 and N == ( N &~ (N-1)));
  for(int rowA = 0; rowA < N; rowA +=2) {
    for(int colB = 0; colB < N; colB += 2){
      int sum = 0;
      for(int iter = 0; iter < N; iter++) 
      {
        sum += matA[rowA * N + iter] * matB[iter * N + colB];
        sum += matA[(rowA+1) * N + iter] * matB[iter * N + colB];
        sum += matA[rowA * N + iter] * matB[iter * N + (colB+1)];
        sum += matA[(rowA+1) * N + iter] * matB[iter * N + (colB+1)];
      }

      // compute output indices
      int rowC = rowA>>1;
      int colC = colB>>1;
      int indexC = rowC * (N>>1) + colC;
      output[indexC] = sum;
    }
  }
}

int main(int argc, char *argv[])
{
  // //Input size of square matrices
  // int N=2048;
  // cout << "Input matrix of size " << N << "\n";

  // int *matA = new int[N * N];
  // int *matB = new int[N * N];
  // // Taking Random Input
  // for(int i=0; i<N; i++){
  //   for(int j=0; j<N; j++){
  //           matA[i*N+j] = (rand() % 10) +1;
  //           matB[i*N+j] = (rand() % 10) +1;
  //       }
  //   }
  string file_name;
  int N;
  cout << "Input matrix of size " << N << "\n"; 
  if (argc < 2) 
    file_name = "data/input_8192.in"; 
  else 
    file_name = argv[1]; 
  ifstream input_file; 
  input_file.open(file_name); 
  input_file >> N;
  cout << "Input matrix of size " << N << "\n";

  // Input matrix A
  int *matA = new int[N * N];
  for(int i = 0; i < N; ++i)
    for(int j = 0; j < N; ++j)
      input_file >> matA[i * N + j];

  // Input matrix B
  int *matB = new int[N * N];
  for(int i = 0; i < N; ++i)
    for(int j = 0; j < N; ++j)
      input_file >> matB[i * N + j];

  // Untimed, warmup caches and TLB
  int *output_reference = new int[(N>>1)*(N>>1)];
  reference(N, matA, matB, output_reference);

  // Execute reference program
  auto begin = TIME_NOW;
  reference(N, matA, matB, output_reference);
  auto end = TIME_NOW;
  cout << "Reference execution time: " << 
    (double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0 << " ms\n";    

  /* if(N<=8){ 
  cerr << "matA: " << endl;
  for(int i=0; i<(N); i++){
    for(int j=0; j<N; j++){
      cerr << matA[i*N+j] << "\t";
    }
    cerr << endl;
  }
  cerr << "matB: " << endl;
  for(int i=0; i<(N); i++){
    for(int j=0; j<N; j++){
      cerr << matB[i*N+j] << "\t";
    }
    cerr << endl;
  }
  cerr << "output_reference: " << endl;
  for(int i=0; i<(N>>1); i++){
    for(int j=0; j<(N>>1); j++){
      cerr << output_reference[i*(N>>1)+j] << "\t";
    }
    cerr << endl;
  }
  /* } */

  // Execute single thread
  /*int *output_single = new int[(N>>1)*(N>>1)];
  begin = TIME_NOW;
  singleThread(N, matA, matB, output_single);
  end = TIME_NOW;
  cout << "Single thread execution time: " << 
    (double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0 << " ms\n";

  for(int i = 0; i < ((N>>1)*(N>>1)); ++i)
    if(output_single[i] != output_reference[i]) {
      cout << "Mismatch at " << i << "\n";
      exit(0);
    }*/

  
    int *block_output_ijk = new int[(N>>1)*(N>>1)];
    int *output_kij = new int[(N>>1)*(N>>1)];
    int *output_ikj = new int[(N>>1)*(N>>1)];
    int *output_single = new int[(N>>1)*(N>>1)];

    auto start = std::chrono::high_resolution_clock::now();
    singleThread(N, matA, matB, output_single);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double>diff = end - start;
    std::cout << "Time to beat " << diff.count() << " s\n";

    start = std::chrono::high_resolution_clock::now();
    singleThreadB_ijk(N, matA, matB, block_output_ijk);
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "Block Time ijk "<< diff.count() << " s\n";

    start = std::chrono::high_resolution_clock::now();
    singleThread_ikj(N, matA, matB, output_ikj);
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "Time of ikj " << diff.count() << " s\n";

    start = std::chrono::high_resolution_clock::now();
    singleThread_kij(N, matA, matB, output_kij);
    end = std::chrono::high_resolution_clock::now();
    diff = end - start;
    std::cout << "Time of kij " << diff.count() << " s\n";
  /*Execute multi-thread
  int *output_multi = new int[(N>>1)*(N>>1)];
  begin = TIME_NOW;
  multiThread(N, matA, matB, output_multi);
  end = TIME_NOW;
  cout << "Multi-threaded execution time: " << 
    (double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0 << " ms\n";

  for(int i = 0; i < ((N>>1)*(N>>1)); ++i)
    if(output_multi[i] != output_reference[i]) {
      cout << "Mismatch at " << i << "\n";
      exit(0);
    }*/

  input_file.close(); 
  return 0; 
}
