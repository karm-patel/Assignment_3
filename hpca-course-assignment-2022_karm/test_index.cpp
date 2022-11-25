#include <iostream>
using namespace std;

int main()
{
    int N = 8;
    float *matrix_a = malloc(N*N*sizeof(float));
    float *matrix_b = malloc(N*N*sizeof(float));
    int result[N][N];

    // initialize matrix_a and matrix_b
    int k=1;
    for (int i = 0; i < N*N; i++) {
        *(matrix_a+i) = k;
        *(matrix_b+i) = k;
        k = k+1;
    }
    // initialize result matrix'
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result[i][j] = 0;
        }
    }
}