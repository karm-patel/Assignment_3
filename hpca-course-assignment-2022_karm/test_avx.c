#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int N;
    scanf("%d",&N);
    printf("Input matrix of size %d \n",N);

    int *matA = malloc(N * N*sizeof(int));
    int *matB = malloc(N * N*sizeof(int));

    // Input matrix A
    for(int i = 0; i < N; ++i)
        for(int j = 0; j < N; ++j)
            scanf("%d",&matA[i * N + j]);
  
    // Input matrix B
    for(int i = 0; i < N; ++i)
        for(int j = 0; j < N; ++j)
            scanf("%d",&matB[i * N + j]);
    int *output = malloc((N>>1)*(N>>1)*sizeof(int));
    __m256i va, vb, vresult;

    int k=1;
    for (int i = 0; i < N*N; i++) {
        *(matA+i) = 1;
        *(matB+i) = 1;
        k = k+1;
    }
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
            printf("%d ",sum);
            sum+=_mm_cvtsi128_si32(sum2);
            printf("%d ",sum);
            sum+=_mm_cvtsi128_si32(sum3);
            printf("%d ",sum);
            sum+=_mm_cvtsi128_si32(sum4);
            printf("%d ",sum);
          }
          // compute output indices
          int rowC = rowA>>1;
          int colC = colB>>1;
          int indexC = rowC * (N>>1) + colC;
          output[indexC] = sum;
          printf("%d \n",sum);
      
    }
  }

  for (int i = 0; i < (N>>1)*(N>>1); i++) 
        printf("%d ", output[i]);
  

    // __m256i rowFirst,rowSecond,colFirst, colSecond,sum1,sum2, sumTotal;
    // for(int rowA=0,rowC=0; rowA<N; rowA+=2,rowC+=(N>>1))
    // {
    //     int rowFirstIndex = rowA*N;
    //     int rowSecondIndex = (rowA+1)*N;
    //     for(int iter=0; iter<N; iter++)
    //     {
    //         rowFirst = _mm256_set1_epi32(matA[rowFirstIndex+iter]);
    //         rowSecond = _mm256_set1_epi32(matA[rowSecondIndex+iter]);
    //         int rowBIndex = iter*N;
    //         for(int colB=0, indexC = rowC; colB<N; colB+=8, indexC+=8)
    //         {
    //             colFirst = _mm256_loadu_si256((__m256i *)&matB[rowBIndex+colB]);
    //             colSecond = _mm256_loadu_si256((__m256i *)&matB[rowBIndex+(colB+8)]);
    //             sum1 = _mm256_add_epi32(_mm256_mullo_epi32(rowFirst,colFirst),_mm256_mullo_epi32(rowSecond,colFirst));
    //             sum2 = _mm256_add_epi32(_mm256_mullo_epi32(rowFirst,colSecond),_mm256_mullo_epi32(rowSecond,colSecond));
    //             printf("%d-%d",sum1,sum2);
    //             sumTotal = _mm256_hadd_epi32(sum1,sum2);
    //             _mm256_storeu_si256((__m256i *)&output[indexC],_mm256_add_epi32(_mm256_loadu_si256((__m256i *)&output[indexC]),sumTotal));
    //             //printf(" \nHi%d ",sumTotal);
    //         }
    //         printf("\n\n");
    //     }
    // }

    // for (int i = 0; i < (N>>1)*(N>>1); i++) 
    //     printf("%d ", output[i]);
    
    return 0;
}