#include <x86intrin.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int N = 8;
    float *matA = malloc(N*N*sizeof(float));
    float *matB = malloc(N*N*sizeof(float));
     int *output = malloc((N>>1)*(N>>1)*sizeof(int));
    __m128 va, vb, vresult;

    // // initialize matrix_a and matrix_b
    // int k=1;
    // for (int i = 0; i < N*N; i++) {
    //     *(matrix_a+i) = k;
    //     *(matrix_b+i) = k;
    //     k = k+1;
    // }
    // // initialize result matrix'
    // for (int i = 0; i < N; i++) {
    //     for (int j = 0; j < N; j++) {
    //         result[i][j] = 0;
    //     }
    // }

    // for (int i = 0; i < N; i++) {
    //     for (int j = 0; j < N; j++) {
    //         for (int k = 0; k < N; k += 4) {
    //             // load
    //             va = _mm_loadu_ps(matrix_a+(i*N)+k); // matrix_a[i][k]
    //             vb = _mm_loadu_ps(matrix_b+(j*N)+k); // matrix_b[j][k]

    //             // multiply
    //             vresult = _mm_mul_ps(va, vb);

    //             // add
    //             vresult = _mm_hadd_ps(vresult, vresult);
    //             vresult = _mm_hadd_ps(vresult, vresult);

    //             // store
    //             result[i][j] += _mm_cvtss_f32(vresult);
    //         }
    //     }
    // }
    
    // for (int i = 0; i < N; i++) {
    //     for (int j = 0; j < N; j++) {
    //         printf("%d ", result[i][j]);
    //     }
    //     printf("\n");
    // }

    int k=1;
    for (int i = 0; i < N*N; i++) {
        *(matA+i) = 1;
        *(matB+i) = 1;
        k = k+1;
    }
    // initialize result matrix'
    for (int i = 0; i < (N>>1)*(N>>1); i++) 
        output[i] = 0;
    __m128 ma1, mb1, ma2, mb2, mresult; 
  /* Registers to keep the data
     ma: to keep matA
     mb: to keep matB
     result: to keep result of inbetween calculation
  */ 
  for(int rowA = 0; rowA < N; rowA +=2) {
    for(int colB = 0; colB < N; colB += 2){
      //for(int iter=0; iter<2; iter++){
      float sum = 0;
          for(int k=0; k<N; k+=4)
          {
            ma1 = _mm_loadu_ps((float *)matA + (rowA*N) + k);
            mb1 = _mm_loadu_ps((float *)matA + (colB*N)+k);
            ma2 = _mm_load1_ps((float *)matA + ((rowA+1)*N) + k);
            mb2 = _mm_loadu_ps((float *)matA + ((colB+1)*N) + k);

            // Multiply ma1 with mb1 and mb2, and then ma2, with mb1 and mb2
            __m128 m1,m2,m3,m4;
            m1 = _mm_mul_ps(ma1, mb1);
            m2 = _mm_mul_ps(ma1, mb2);
            m3 = _mm_mul_ps(ma2, mb1);
            m4 = _mm_mul_ps(ma2, mb2);

            // add
            m1 = _mm_hadd_ps(m1, m1);
            m1 = _mm_hadd_ps(m1, m1);

            m2 = _mm_hadd_ps(m2, m2);
            m2 = _mm_hadd_ps(m2, m2);

            m3 = _mm_hadd_ps(m3, m3);
            m3 = _mm_hadd_ps(m3, m3);

            m4 = _mm_hadd_ps(m4, m4);
            m4 = _mm_hadd_ps(m4, m4);

            sum += _mm_cvtss_f32(m1);
            printf("Hello %d ", sum);
            sum += _mm_cvtss_f32(m2);
            sum += _mm_cvtss_f32(m3);
            sum += _mm_cvtss_f32(m4);
          }
          // compute output indices
          int rowC = rowA>>1;
          int colC = colB>>1;
          int indexC = rowC * (N>>1) + colC;
          output[indexC] = sum;
          printf("%f ",sum);
    }
  }
    
    return 0;
}