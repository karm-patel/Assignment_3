#include <x86intrin.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int N = 256;
    float *matrix_a = malloc(N*N*sizeof(float));
    float *matrix_b = malloc(N*N*sizeof(float));
    int result[N][N];
    __m128 va, vb, vresult;

    // initialize matrix_a and matrix_b
    for (int i = 0; i < N*N; i++) {
        *(matrix_a+i) = 1;
        *(matrix_b+i) = 2;
    }
    // initialize result matrix'
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result[i][j] = 0;
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k += 4) {
                // load
                va = _mm_loadu_ps(matrix_a+(i*N)+k); // matrix_a[i][k]
                vb = _mm_loadu_ps(matrix_b+(j*N)+k); // matrix_b[j][k]

                // multiply
                vresult = _mm_mul_ps(va, vb);

                // add
                vresult = _mm_hadd_ps(vresult, vresult);
                vresult = _mm_hadd_ps(vresult, vresult);

                // store
                result[i][j] += _mm_cvtss_f32(vresult);
            }
        }
    }
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}