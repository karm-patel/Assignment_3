#include <immintrin.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
void singleThread(int N, int *matA, int *matB, int *output)
{
  if(N<=16)
  {
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

  else{
  __m512i r1,r2,r3,r4;
  for(int i=0; i<(N>>1*N>>1); i++)
    output[i] = 0;
    for(int rowA=0; rowA<N; rowA+=2)
    {
      for(int iter=0; iter<N; iter+=2)
        {
          int r1idx = rowA*N;
          int r2idx = (rowA+1)*N;
            // to distribute data across peices
            union{
                int sum[8];
                __m512i sum_all;
            };
            r1 = _mm512_set1_epi32(matA[r1idx+iter]);
            r2 = _mm512_set1_epi32(matA[r2idx+iter]);
            r3 = _mm512_set1_epi32(matA[r1idx+(iter+1)]);
            r4 = _mm512_set1_epi32(matA[r2idx+(iter+1)]);
            int rowC = rowA >> 1;
            int rowIndex = rowC*(N>>1);
            for(int colB=0; colB<N; colB+=16)
            {
                __m512i col1 = _mm512_loadu_si512((__m512i *)&matB[(iter)*N + colB]);
                __m512i col2 = _mm512_loadu_si512((__m512i *)&matB[(iter+1)*N + colB]);
                __m512i m1 = _mm512_mullo_epi32(r1,col1);
                __m512i m2 = _mm512_mullo_epi32(r2,col1);
                __m512i m3 = _mm512_mullo_epi32(r3,col2);
                __m512i m4 = _mm512_mullo_epi32(r4,col2);
                sum_all = _mm512_add_epi32(_mm512_add_epi32(m1,m2), _mm512_add_epi32(m3,m4));
                int colC = colB>>1;
                int indexC = rowIndex + colC;
                output[indexC] += sum[0] + sum[1];
                output[indexC+1] += sum[2] + sum[3];
                output[indexC+2] += sum[4] + sum[5];
                output[indexC+3] += sum[6] + sum[7];
                // cout << output[indexC];
            }
        }
    }
  }
}