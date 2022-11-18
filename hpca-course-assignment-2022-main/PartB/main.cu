#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>
#include <assert.h>
using namespace std;

#include "gpu_thread.h"

#define TIME_NOW std::chrono::high_resolution_clock::now()
#define TIME_DIFF(gran, start, end) std::chrono::duration_cast<gran>(end - start).count()

// Used to cross-check answer. DO NOT MODIFY!
void reference(int N, int *matA, int *matB, int *output)
{
  // enforce N to be power of 2 and greater than 2
  assert( N>=4 and N == ( N &~ (N-1)));
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
    // Input size of square matrices// Input size of square matrices
    int N;
    string file_name; 
    if (argc < 2) 
        file_name = "data/input_128.in"; 
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
    
    // Execute reference program
    int *output_reference = new int[N*(N>>1)];
    auto begin = TIME_NOW;
    // reference(N, matA, matB, output_reference);
    auto end = TIME_NOW;
    cout << "Reference execution time: " << 
      (double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0 << " ms\n";  
    
    // Execute gpu version
    int *output_gpu = new int[N*(N>>1)];
    begin = TIME_NOW;
    gpuThread(N, matA, matB, output_gpu);
    end = TIME_NOW;
    cout << "GPU execution time: " << 
      (double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0 << " ms\n"; 
    
    
    // for(int i = 0; i < N*(N>>1); ++i)
    //     if(output_gpu[i] != output_reference[i]) {
    //         cout << "Mismatch at " << i << "\n";
    //         cout << "GPU output: " << output_gpu[i] << ", required output: " << output_reference[i] << "\n";
    //         exit(0);
    //     }
    input_file.close(); 
}
