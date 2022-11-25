#include <pthread.h>

// Create other necessary functions here

// to pass arguments in function
struct Arguments {
  int N;
  int row_start;
  int col_start;
  int *matA;
  int *matB;
  int *output;
};

void printArray(int N, int *arr){
    printf("\nMULTI THREAD ARRAY:\n");
    for(int i = 0; i < N; i ++){
        for(int j = 0; j < N; j ++){
        printf("%d ", arr[i*N + j]);
        }
    printf("\n");
  }
  printf("-------------------------\n");
}

// initailize arr with 0
void ini_zero(int N, int *arr){
    for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			arr[i*N + j] = 0;
		}
	}
}

// Modified single thread multiplication
void * multiThread_struct(void * args)
{
  int N, row_start, col_start;
  int * matA, * matB, * output;
  struct Arguments * arg = (struct Arguments *) args;
  N = arg->N;
  row_start = arg->row_start;
  col_start = arg->col_start;
  matA = arg->matA;
  matB = arg->matB;
  output = arg->output;

  for(int rowA = row_start; rowA < row_start+N/2; rowA +=2) {
    for(int colB = col_start; colB < col_start+N/2; colB += 2){
      int sum = 0;
      for(int iter = 0; iter < N; iter++) 
      {
        sum += matA[rowA * N + iter] * matB[iter * N + colB];
        sum += matA[(rowA+1) * N + iter] * matB[iter * N + colB];
        sum += matA[rowA * N + iter] * matB[iter * N + (colB+1)];
        sum += matA[(rowA+1) * N + iter] * matB[iter * N + (colB+1)];

      // compute output indices
      int rowC = rowA>>1;
      int colC = colB>>1;
      int indexC = rowC * (N>>1) + colC;
    //   printf("%d ", indexC);
      output[indexC] = sum;
    }
  }
} 
}


// Fill in this function
void multiThread(int N, int *matA, int *matB, int *output)
{
    ini_zero(N/2, output);
    int N_THREADS = 4; 
    pthread_t threads[N_THREADS];
    struct Arguments args[N_THREADS];
    struct Arguments * args_ptrs[N_THREADS];

    for(int i =0; i<N_THREADS; i++){
      args[i].N = N;
      args[i].row_start = (i/2)*N/2;
      args[i].col_start = (i%2)*N/2;
      args[i].matA = matA;
      args[i].matB = matB;
      args[i].output = output;
    
      args_ptrs[i] = &args[i];
    }

    for(int i =0; i<N_THREADS; i++){
      pthread_create( &threads[i], NULL, multiThread_struct, (void*) args_ptrs[i]);
    }

    for(int i =0; i<N_THREADS; i++){
      pthread_join(threads[i], NULL);
    }

    // printArray(N/2, output);
}
