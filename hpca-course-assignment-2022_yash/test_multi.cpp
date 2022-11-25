#include <pthread.h>
#include <immintrin.h>
#include <iostream>
#include <ctime>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define TIME_NOW std::chrono::high_resolution_clock::now()
#define TIME_DIFF(gran, start, end) std::chrono::duration_cast<gran>(end - start).count()

int *matA;
int *matB;
int *output;
struct thread_args{
    int rowS;
    int colS;
    int N;
};
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// Create other necessary functions here
void *helper(void *args)
{
    struct thread_args * arg = (struct thread_args *) args;
    int N = arg->N;
    int rowS = arg->rowS;
    int colS = arg->colS;
  __m256i r1,r2,r3,r4;
    for(int rowA=rowS; rowA<rowS+N/2; rowA+=2)
    {
      for(int iter=0; iter<N; iter+=2)
        {
          int r1idx = rowA*N;
          int r2idx = (rowA+1)*N;
            // to distribute data across peices
            union{
                int sum[8];
                __m256i sum_all;
            };
            r1 = _mm256_set1_epi32(matA[r1idx+iter]);
            r2 = _mm256_set1_epi32(matA[r2idx+iter]);
            r3 = _mm256_set1_epi32(matA[r1idx+(iter+1)]);
            r4 = _mm256_set1_epi32(matA[r2idx+(iter+1)]);
            int rowC = rowA >> 1;
            int rowIndex = rowC*(N>>1);
            for(int colB=colS; colB<colS+N/2; colB+=8)
            {
                __m256i col1 = _mm256_loadu_si256((__m256i *)&matB[(iter)*N + colB]);
                __m256i col2 = _mm256_loadu_si256((__m256i *)&matB[(iter+1)*N + colB]);
                __m256i m1 = _mm256_mullo_epi32(r1,col1);
                __m256i m2 = _mm256_mullo_epi32(r2,col1);
                __m256i m3 = _mm256_mullo_epi32(r3,col2);
                __m256i m4 = _mm256_mullo_epi32(r4,col2);
                sum_all = _mm256_add_epi32(_mm256_add_epi32(m1,m2), _mm256_add_epi32(m3,m4));
                int colC = colB>>1;
                int indexC = rowIndex + colC;
                long private_thread_t1 = sum[0] + sum[1];
                long private_thread_t2 = sum[2] + sum[3];
                long private_thread_t3 = sum[4] + sum[5];
                long private_thread_t4 = sum[6] + sum[7];
                //pthread_mutex_lock(&lock);
                output[indexC] += private_thread_t1;
                output[indexC+1] += private_thread_t2;
                output[indexC+2] += private_thread_t3;
                output[indexC+3] += private_thread_t4;
                //pthread_mutex_unlock(&lock);
                //cout << output[indexC] << " ";
            }
        }
    }
}

// Fill in this function
void multiThread(int N, int *matA, int *matB, int *output)
{
    if(N<=8)
    {
      for(int rowA = 0; rowA < N; rowA +=2) {
        for(int colB = 0; colB < N; colB += 2){
          int sum = 0;
          for(int iter = 0; iter < N; iter++) {
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
    else{
        // Assigning variables to global
        ::matA = matA;
        ::matB = matB;
        for(int i=0; i<(N>>1*N>>1); i++)
            output[i] = 0;
        ::output = output;

        int N_THREADS = 4; 
        pthread_t threads[N_THREADS];
        struct thread_args args[N_THREADS];
        struct thread_args * args_ptrs[N_THREADS];

        for(int k=0; k<N/N_THREADS; k+=N_THREADS){
          for(int i=0; i<N_THREADS; i++){
              args[i].N = N;
              args[i].rowS = (i/2)*N/2;
              args[i].colS = (i%2)*N/2;
              args_ptrs[i] = &args[i];
          }
        }

        for(int i=0; i<N_THREADS; i++){
            pthread_create( &threads[i], NULL, helper, (void*) args_ptrs[i]);
        }

        for(int i=0; i<N_THREADS; i++){
            pthread_join(threads[i], NULL);
        }
    }
}

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
int main()
{
    int N;
    scanf("%d",&N);
    printf("Input matrix of size %d \n",N);

    int *matA = new int[N*N];
    int *matB = new int[N*N];
    int k = 1;
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            matA[i*N+j] = k;
            matB[i*N+j] = k;
        }
        k++;
    }
    // Untimed, warmup caches and TLB
    int *output_multi = new int[(N>>1)*(N>>1)];
    int *output_reference = new int[(N>>1)*(N>>1)];

    // Execute reference program
    // auto begin = TIME_NOW;
    // reference(N, matA, matB, output_reference);
    // auto end = TIME_NOW;
    // cout << "Reference execution time: " << 
    //     (double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0 << " ms\n"; 
    //     int *output_vector = new int[(N>>1)*(N>>1)];

    
    auto begin = TIME_NOW;
    multiThread(N, matA, matB, output_multi);
    auto end = TIME_NOW;
    cout << "Multi execution time: " << 
        (double)TIME_DIFF(std::chrono::microseconds, begin, end) / 1000.0 << " ms\n"; 

    // for(int i = 0; i < ((N>>1)*(N>>1)); ++i)
    // if(output_multi[i] != output_reference[i]) {
    //   cout << "Mismatch at " << i << "\n";
    //   exit(0);
    // }
    return 0;
}