#include <pthread.h>
#include <immintrin.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// int *matA;
// int *matB;
// int *output;
// struct thread_args{
//     int rowS;
//     int colS;
//     int N;
// };
// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// // Create other necessary functions here
// void *helper(void *args)
// {
//     struct thread_args * arg = (struct thread_args *) args;
//     int N = arg->N;
//     int rowS = arg->rowS;
//     int colS = arg->colS;
//   __m256i r1,r2,r3,r4;
//     for(int rowA=rowS; rowA<rowS+N/2; rowA+=2)
//     {
//       for(int iter=0; iter<N; iter+=2)
//         {
//           int r1idx = rowA*N;
//           int r2idx = (rowA+1)*N;
//             // to distribute data across peices
//             union{
//                 int sum[8];
//                 __m256i sum_all;
//             };
//             r1 = _mm256_set1_epi32(matA[r1idx+iter]);
//             r2 = _mm256_set1_epi32(matA[r2idx+iter]);
//             r3 = _mm256_set1_epi32(matA[r1idx+(iter+1)]);
//             r4 = _mm256_set1_epi32(matA[r2idx+(iter+1)]);
//             int rowC = rowA >> 1;
//             int rowIndex = rowC*(N>>1);
//             for(int colB=colS; colB<colS+N/2; colB+=8)
//             {
//                 __m256i col1 = _mm256_loadu_si256((__m256i *)&matB[(iter)*N + colB]);
//                 __m256i col2 = _mm256_loadu_si256((__m256i *)&matB[(iter+1)*N + colB]);
//                 __m256i m1 = _mm256_mullo_epi32(r1,col1);
//                 __m256i m2 = _mm256_mullo_epi32(r2,col1);
//                 __m256i m3 = _mm256_mullo_epi32(r3,col2);
//                 __m256i m4 = _mm256_mullo_epi32(r4,col2);
//                 sum_all = _mm256_add_epi32(_mm256_add_epi32(m1,m2), _mm256_add_epi32(m3,m4));
//                 int colC = colB>>1;
//                 int indexC = rowIndex + colC;
//                 long private_thread_t1 = sum[0] + sum[1];
//                 long private_thread_t2 = sum[2] + sum[3];
//                 long private_thread_t3 = sum[4] + sum[5];
//                 long private_thread_t4 = sum[6] + sum[7];
//                 pthread_mutex_lock(&lock);
//                 output[indexC] += private_thread_t1;
//                 output[indexC+1] += private_thread_t2;
//                 output[indexC+2] += private_thread_t3;
//                 output[indexC+3] += private_thread_t4;
//                 pthread_mutex_unlock(&lock);
//                 // cout << output[indexC];
//             }
//         }
//     }
// }

// // Fill in this function
// void multiThread(int N, int *matA, int *matB, int *output)
// {
//     if(N<=16)
//     {
//       for(int rowA = 0; rowA < N; rowA +=2) {
//         for(int colB = 0; colB < N; colB += 2){
//           int sum = 0;
//           for(int iter = 0; iter < N; iter++) {
//             sum += matA[rowA * N + iter] * matB[iter * N + colB];
//             sum += matA[(rowA+1) * N + iter] * matB[iter * N + colB];
//             sum += matA[rowA * N + iter] * matB[iter * N + (colB+1)];
//             sum += matA[(rowA+1) * N + iter] * matB[iter * N + (colB+1)];
//           }
//             // compute output indices
//             int rowC = rowA>>1;
//             int colC = colB>>1;
//             int indexC = rowC * (N>>1) + colC;
//             output[indexC] = sum;
//             }
//         }
//     }
//     else{
//         // Assigning variables to global
//         ::matA = matA;
//         ::matB = matB;
//         for(int i=0; i<(N>>1*N>>1); i++)
//             output[i] = 0;
//         ::output = output;

//         int N_THREADS = 4; 
//         pthread_t threads[N_THREADS];
//         struct thread_args args[N_THREADS];
//         struct thread_args * args_ptrs[N_THREADS];

//         for(int i=0; i<N_THREADS; i++){
//             args[i].N = N;
//             args[i].rowS = (i/2)*N/2;
//             args[i].colS = (i%2)*N/2;
//             args_ptrs[i] = &args[i];
//         }

//         for(int i=0; i<N_THREADS; i++){
//             pthread_create( &threads[i], NULL, helper, (void*) args_ptrs[i]);
//         }

//         for(int i=0; i<N_THREADS; i++){
//             pthread_join(threads[i], NULL);
//         }
//     }
// }


/**** Multi threading tiling*/
int *matA;
int *matB;
int *output;
int N;
struct thread_args{
    int rowS;
    int colS;
    int outS;
};
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// Create other necessary functions here
void *helper(void *args)
{
    struct thread_args * arg = (struct thread_args *) args;
  __m256i r1,r2,r3,r4;
    // for(int rowA=rowS; rowA<rowS+N/2; rowA+=2)
    // {
    for(int k=0; k<N/4; k+=64){
        for(int i=0; i<N/4; i+=64){
            for(int j=0; j<N/4; j+=64){
                for(int iter=0; iter<64; iter+=2){
                    for(int rowA=0; rowA<64; rowA+=2){
                        int r1idx = (arg->rowS)+(i*N+k)+(rowA*N);
                        int r2idx = (arg->rowS)+(i*N+k)+((rowA+1)*N);
                        // to distribute data across peices
                        union{
                            int sum[8];
                            __m256i sum_all;
                        };
                        r1 = _mm256_set1_epi32(matA[r1idx+iter]);
                        r2 = _mm256_set1_epi32(matA[r2idx+iter]);
                        r3 = _mm256_set1_epi32(matA[r1idx+(iter+1)]);
                        r4 = _mm256_set1_epi32(matA[r2idx+(iter+1)]);
                        int rowC = rowA >> 1; //((arg->rowS)+(i*N)+rowA) >> 1;
                        int rowIndex = rowC*(N>>1);
                        for(int colB=0; colB<64; colB+=8){
                            __m256i col1 = _mm256_loadu_si256((__m256i *)&matB[arg->colS+(k*N+j)+(iter*N) + colB]);
                            __m256i col2 = _mm256_loadu_si256((__m256i *)&matB[arg->colS+(k*N+j)+((iter+1)*N) + colB]);
                            __m256i m1 = _mm256_mullo_epi32(r1,col1);
                            __m256i m2 = _mm256_mullo_epi32(r2,col1);
                            __m256i m3 = _mm256_mullo_epi32(r3,col2);
                            __m256i m4 = _mm256_mullo_epi32(r4,col2);
                            sum_all = _mm256_add_epi32(_mm256_add_epi32(m1,m2), _mm256_add_epi32(m3,m4));
                            int colC = colB>>1;
                            int indexC = arg->outS + (rowIndex + colC); //((i*(N>>1))+j);
                            //cout << indexC << " ";
                            output[indexC] += sum[0] + sum[1];
                            output[indexC+1] += sum[2] + sum[3];
                            output[indexC+2] += sum[4] + sum[5];
                            output[indexC+3] += sum[6] + sum[7];
                        }
                    }
                }
            }
        }
    }
}

// Fill in this function
void multiThread(int N, int *matA, int *matB, int *output)
{
    if(N<=128)
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
        ::N = N;
        for(int i=0; i<(N>>1*N>>1); i++)
            output[i] = 0;
        ::output = output;

        int N_THREADS = 64; 
        pthread_t threads[N_THREADS];
        //struct thread_args args[N_THREADS];
        //struct thread_args * args_ptrs[N_THREADS];
        struct thread_args * args;
        // Creating 64 threads each covering (N/4)*(N/4) area of matrix
        for(int j=0; j<4 ; j++){
            for(int i=0; i<N_THREADS/4; i++){
                args = new thread_args;         
                args->rowS = (((i/4)*(N/4))*(N)) + ((j%4)*(N/4));
                args->colS = (((j%4)*(N/4))*(N)) + ((i%4)*(N/4));
                args->outS = (((i/4)*(N/8))*(N/2)) + ((i%4)*(N/8));
                //args_ptrs[i] = &args[i];
                pthread_create(&threads[(j*(N_THREADS/4))+i], NULL, helper, (void *) args);            
            }
            for(int k=0; k<N_THREADS/4; k++)
                pthread_join(threads[(j*(N_THREADS/4))+k], NULL);
        }
    }
}