// Optimize this function

void singleThread(int N, int *matA, int *matB, int *output)
{
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

  printf("ARRAY: ");
    for(int i = 0; i < N; i ++){
    for(int j = 0; j < N; j ++){
      printf("%d ", *(output + i*N + j));
    }
    printf("\n");
  }
}

void singleThread_kij(int N, int *matA, int *matB, int *output)
{

  for(int i = 0; i < N; i ++){
    for(int j = 0; j < N; j ++){
      output[i*N + j] = 0;
    }
  }

  assert( N>=4 and N == ( N &~ (N-1)));
  for(int iter = 0; iter < N; iter ++) {
    for(int rowA = 0; rowA < N; rowA += 2){
      int sum = 0;
      for(int colB = 0; colB < N; colB+=2) 
      {
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

  printf("ARRAY kij: ");
      for(int i = 0; i < N; i ++){
    for(int j = 0; j < N; j ++){
      printf("%d ", output[i*N + j]);
    }
    printf("\n");
  }

}
