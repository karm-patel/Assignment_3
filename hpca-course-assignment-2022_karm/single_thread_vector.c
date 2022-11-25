// Optimize this function
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
// #include <iostream>
// #include <ctime>
// #include <chrono>
// #include <fstream>
#include <assert.h>
// #include <iomanip>


// #define TIME_NOW std::chrono::system_clock::now()
// #define TIME_DIFF(gran, start, end) std::chrono::duration_cast<gran>(end - start).count()

// void singleThread(int N, int *matA, int *matB, int *output)
// {
//   //assert( N>=4 and N == ( N &~ (N-1)));
  
//   for(int rowA = 0; rowA < N; rowA +=2) {
//     for(int colB = 0; colB < N; colB += 2){
//       int sum = 0;
//       for(int iter = 0; iter < N; iter++) 
//       {
//         sum += matA[rowA * N + iter] * matB[iter * N + colB];
//         sum += matA[(rowA+1) * N + iter] * matB[iter * N + colB];
//         sum += matA[rowA * N + iter] * matB[iter * N + (colB+1)];
//         sum += matA[(rowA+1) * N + iter] * matB[iter * N + (colB+1)];
//       }

//       // compute output indices
//       int rowC = rowA>>1;
//       int colC = colB>>1;
//       int indexC = rowC * (N>>1) + colC;
//       output[indexC] = sum;
//       cout << sum << " ";
//     }
//   }
// }

void singleThread_vector(int N, int *matA, int *matB, int *output)
{
  // initialize result matrix'
    for (int i = 0; i < (N>>1)*(N>>1); i++) 
        output[i] = 0;
    __m256i ma1, mb1, ma2, mb2, mresult;
    __m256i m1,m2,m3,m4;
    __m128i sum1,sum2,sum3,sum4; 
  /* Registers to keep the data
     ma: to keep matA
     mb: to keep matB
     result: to keep result of inbetween calculation
  */ 
  for(int rowA = 0; rowA < N; rowA +=2) {
    for(int colB = 0; colB < N; colB += 2){
        int sum = 0;
    //   for(int iter=0; iter<N; iter++){
      
        //   for(int k=0; k<N; k+=8)
        //   {
            for(int iter=0; iter<N; iter+=8){
            ma1 = _mm256_loadu_si256(matA + (rowA*N) + iter);
            mb1 = _mm256_loadu_si256(matB + (colB*N) + iter);
            ma2 = _mm256_loadu_si256(matA + ((rowA+1)*N) + iter);
            mb2 = _mm256_loadu_si256(matB + ((colB+1)*N) + iter);

            // Multiply ma1 with mb1 and mb2, and then ma2, with mb1 and mb2
            
            m1 = _mm256_mullo_epi32(ma1, mb1);
            m2 = _mm256_mullo_epi32(ma1, mb2);
            m3 = _mm256_mullo_epi32(ma2, mb1);
            m4 = _mm256_mullo_epi32(ma2, mb2);

            // add
            sum1 = _mm_add_epi32(_mm256_castsi256_si128(m1), _mm256_extractf128_si256(m1,1));
            sum1 = _mm_hadd_epi32(sum1,sum1);
            sum1 = _mm_hadd_epi32(sum1,sum1);

            sum2 = _mm_add_epi32(_mm256_castsi256_si128(m2), _mm256_extractf128_si256(m2,1));
            sum2 = _mm_hadd_epi32(sum2,sum2);
            sum2 = _mm_hadd_epi32(sum2,sum2);

            sum3 = _mm_add_epi32(_mm256_castsi256_si128(m3), _mm256_extractf128_si256(m3,1));
            sum3 = _mm_hadd_epi32(sum3,sum3);
            sum3 = _mm_hadd_epi32(sum3,sum3);

            sum4 = _mm_add_epi32(_mm256_castsi256_si128(m4), _mm256_extractf128_si256(m4,1));
            sum4 = _mm_hadd_epi32(sum4,sum4);
            sum4 = _mm_hadd_epi32(sum4,sum4);

            sum+=_mm_cvtsi128_si32(sum1);
            // printf("%d ",sum);
            sum+=_mm_cvtsi128_si32(sum2);
            // printf("%d ",sum);
            sum+=_mm_cvtsi128_si32(sum3);
            // printf("%d ",sum);
            sum+=_mm_cvtsi128_si32(sum4);
            // printf("%d ",sum);
          }
          // compute output indices
          int rowC = rowA>>1;
          int colC = colB>>1;
          int indexC = rowC * (N>>1) + colC;
          output[indexC] = sum;
          // printf("%d \n",sum);
      
    }
  }
}

// void singleThreadB_ijk(int N, int *matA, int *matB, int *output)
// {
//   int blockSize = 4;
//   //assert( N>=4 and N == ( N &~ (N-1)));
//   for(int i = 0; i < ((N>>1)*(N>>1)); ++i)
//       output[i] = 0;
//   for(int i=0; i<N; i+=blockSize)
//     {
//         for(int j=0; j<N; j+=blockSize)
//         {
//             //for (int k=0; k<N; k+=blockSize)
//             //{
//                 for(int rowA=i; rowA < i+blockSize; rowA+=2)
//                 {
//                     for(int colB=j; colB<j+blockSize; colB+=2)
//                     {
//                       int sum = 0;
//                       for(int iter = 0; iter<blockSize; iter++) 
//                       {
//                         sum += matA[rowA * N + iter] * matB[iter * N + colB];
//                         sum += matA[(rowA+1) * N + iter] * matB[iter * N + colB];
//                         sum += matA[rowA * N + iter] * matB[iter * N + (colB+1)];
//                         sum += matA[(rowA+1) * N + iter] * matB[iter * N + (colB+1)];
//                       }
//                       // compute output indices
//                       int rowC = i>>1;
//                       int colC = j>>1;
//                       int indexC = rowC * (N>>1) + colC;
//                       cout << "I: " << indexC << " ";
//                       output[indexC] += sum;
//                       cout << sum << " ";
//                     }
//                 }
//             //}
//         }
//     }

//     cout << "O\n";
//     for(int i = 0; i < ((N>>1)*(N>>1)); ++i)
//       cout << output[i] << " ";
// }

// void singleThreadB_ikj(int N, int *matA, int *matB, int *output)
// {
//     int blockSize = 4;
//   //assert( N>=4 and N == ( N &~ (N-1)));
//   for(int rowA=0; rowA<N; rowA +=blockSize)
//     {
//         for (int k=0; k < N; k = k + blockSize)
//         {
//             int sum = 0;
//             for(int colB=0; colB<N; colB = colB + blockSize)
//             {
//                 for(int ii=rowA; ii<rowA+blockSize; ii++)
//                 {
//                     for(int jj=colB; jj < colB+blockSize; jj++)
//                     {
//                         for(int kk=k; kk < k+blockSize; kk++)
//                         {
//                             sum += matA[ii*N + kk] * matB[kk*N+jj];
//                         }
//                     }
//                 }
//                 // compute output indices
//                 int rowC = rowA>>1;
//                 int colC = colB>>1;
//                 int indexC = rowC * (N>>1) + colC;
//                 output[indexC] += sum;
//             }
//         }
//     }
// }

// void singleThread_kij(int N, int *matA, int *matB, int *output)
// {
//   for(int i = 0; i < ((N>>1)*(N>>1)); ++i)
//       output[i] = 0;
//   for(int iter = 0; iter < N; iter ++) {
//     for(int rowA = 0; rowA < N; rowA +=2){
//       for(int colB = 0; colB < N; colB+=2) 
//       {
//         int sum = 0;
//         sum += matA[rowA * N + iter] * matB[iter * N + colB];
//         sum += matA[(rowA+1) * N + iter] * matB[iter * N + colB];
//         sum += matA[rowA * N + iter] * matB[iter * N + (colB+1)];
//         sum += matA[(rowA+1) * N + iter] * matB[iter * N + (colB+1)];
       
//         //compute output indices
//         int rowC = rowA>>1;
//         int colC = colB>>1;
//         int indexC = rowC * (N>>1) + colC;
//         output[indexC] += sum;
//       }
//     }
//   }
// }

// void singleThread_ikj(int N, int *matA, int *matB, int *output)
// {
//   for(int i = 0; i < ((N>>1)*(N>>1)); ++i)
//       output[i] = 0;
//   for(int rowA = 0; rowA < N; rowA += 2) {
//     for(int iter = 0; iter < N; iter ++){
//       for(int colB = 0; colB < N; colB+=2) 
//       {
//         int sum = 0;
//         sum += matA[rowA * N + iter] * matB[iter * N + colB];
//         sum += matA[(rowA+1) * N + iter] * matB[iter * N + colB];
//         sum += matA[rowA * N + iter] * matB[iter * N + (colB+1)];
//         sum += matA[(rowA+1) * N + iter] * matB[iter * N + (colB+1)];
       
//         //compute output indices
//         int rowC = rowA>>1;
//         int colC = colB>>1;
//         int indexC = rowC * (N>>1) + colC;
//         output[indexC] += sum;
//       }
//     }
//   }
  
// }

// void reference(int N, int *matA, int *matB, int *output)
// {
//   // enforce N to be power of 2 and greater than 2
//   //assert( N>=4 and N == ( N &~ (N-1)));
//   for(int rowA = 0; rowA < N; rowA +=2) {
//     for(int colB = 0; colB < N; colB += 2){
//       int sum = 0;
//       for(int iter = 0; iter < N; iter++) 
//       {
//         sum += matA[rowA * N + iter] * matB[iter * N + colB];
//         sum += matA[(rowA+1) * N + iter] * matB[iter * N + colB];
//         sum += matA[rowA * N + iter] * matB[iter * N + (colB+1)];
//         sum += matA[(rowA+1) * N + iter] * matB[iter * N + (colB+1)];
//       }

//       // compute output indices
//       int rowC = rowA>>1;
//       int colC = colB>>1;
//       int indexC = rowC * (N>>1) + colC;
//       output[indexC] = sum;
//     }
//   }
// }

// void printArray(int N, int *output)
// {
//   for(int i = 0; i < ((N>>1)*(N>>1)); ++i)
//       cout << output[i] << " ";
// }

int main(int argc, char *argv[])
{
  // Input size of square matrices
  int N;
  scanf("%d",&N);
  printf("Input matrix of size %d \n",N);

    int *matA = malloc(N * N*sizeof(int));
    int *matB = malloc(N * N*sizeof(int));
  // int *matA = new int[N * N];
  // int *matB = new int[N * N];

  // // Input matrix A
  // for(int i = 0; i < N; ++i)
  //   for(int j = 0; j < N; ++j)
  //     cin >> matA[i * N + j];
  
  // // Input matrix B
  // for(int i = 0; i < N; ++i)
  //   for(int j = 0; j < N; ++j)
  //     cin >> matB[i * N + j];

  // Taking Random Input
  for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
            matA[i*N+j] = (rand() % 10) +1;
            matB[i*N+j] = (rand() % 10) +1;
        }
    }


  // Untimed, warmup caches and TLB
  // int *output_reference = new int[(N>>1)*(N>>1)];
  // reference(N, matA, matB, output_reference);

  //   // Execute single thread
  //   int *block_output_ijk = new int[(N>>1)*(N>>1)];
  //   int *block_output_ikj = new int[(N>>1)*(N>>1)];
  //   int *output_kij = new int[(N>>1)*(N>>1)];
  //   int *output_ikj = new int[(N>>1)*(N>>1)];
  //   int *output_single = new int[(N>>1)*(N>>1)];
    int *output_vector = new int[(N>>1)*(N>>1)];

    // auto start = std::chrono::high_resolution_clock::now();
    // singleThread(N, matA, matB, output_single);
    // auto end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double>diff = end - start;
    // std::cout << "Time to beat " << diff.count() << " s\n";

    auto start = std::chrono::high_resolution_clock::now();
    singleThread_vector(N, matA, matB, output_vector);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double>diff = end - start;
    std::cout << "Block Time ijk "<< diff.count() << " s\n";

    // start = std::chrono::high_resolution_clock::now();
    // singleThreadB_ijk(N, matA, matB, block_output_ijk);
    // end = std::chrono::high_resolution_clock::now();
    // diff = end - start;
    // std::cout << "Block Time ijk "<< diff.count() << " s\n";

    // start = std::chrono::high_resolution_clock::now();
    // singleThreadB_ikj(N, matA, matB, block_output_ikj);
    // end = std::chrono::high_resolution_clock::now();
    // diff = end - start;
    // std::cout << "Block Time ikj " << diff.count() << " s\n";

    // start = std::chrono::high_resolution_clock::now();
    // singleThread_ikj(N, matA, matB, output_ikj);
    // end = std::chrono::high_resolution_clock::now();
    // diff = end - start;
    // std::cout << "Time of ikj " << diff.count() << " s\n";

    // start = std::chrono::high_resolution_clock::now();
    // singleThread_kij(N, matA, matB, output_kij);
    // end = std::chrono::high_resolution_clock::now();
    // diff = end - start;
    // std::cout << "Time of kij " << diff.count() << " s\n";

  //for(int i = 0; i < ((N>>1)*(N>>1)); ++i)
    //cout << my_output[i] << " ";   
  // for(int i = 0; i < ((N>>1)*(N>>1)); ++i)
  //   if(output_single[i] != block_output_ijk[i]) {
  //     cout << "Mismatch of block ijk at " << i << "\n";
  //     //exit(0);
  //   }
  // for(int i = 0; i < ((N>>1)*(N>>1)); ++i)
  //   if(output_single[i] != block_output_ikj[i]) {
  //     cout << "Mismatch of block ikj at " << i << "\n";
  //     //exit(0);
  //   }

  // for(int i = 0; i < ((N>>1)*(N>>1)); ++i)
  //   if(output_single[i] != output_ikj[i]) {
  //     cout << "Mismatch of ikj at " << i << "\n";
  //     exit(0);
  //   }

  // for(int i = 0; i < ((N>>1)*(N>>1)); ++i)
  //   if(output_single[i] != output_kij[i]) {
  //     cout << "Mismatch of kij at " << i << "\n";
  //     exit(0);
  //   }

  return 0; 
}

