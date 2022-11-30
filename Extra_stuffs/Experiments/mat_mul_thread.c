#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<pthread.h>


struct Arguments {
  int N;
  int row_start;
  int col_start;
  int *matA;
  int *matB;
  int *output;
};

void printArray(int N, int *arr){
    printf("\nARRAY:\n");
    for(int i = 0; i < N; i ++){
        for(int j = 0; j < N; j ++){
        printf("%d ", arr[i*N + j]);
        }
    printf("\n");
  }
  printf("-------------------------\n");
}

void singleThread(int N, int *matA, int *matB, int *output)
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

  printArray(N/2, output);

}

void multiThread(int N, int row_start, int col_start, int *matA, int *matB, int *output)
{

  for(int rowA = row_start; rowA < row_start+N/2; rowA +=2) {
    for(int colB = col_start; colB < col_start+N/2; colB += 2){
      int sum = 0;
      for(int iter = 0; iter < N; iter++) 
      {
        sum += matA[rowA * N + iter] * matB[iter * N + colB];
        sum += matA[(rowA+1) * N + iter] * matB[iter * N + colB];
        sum += matA[rowA * N + iter] * matB[iter * N + (colB+1)];
        sum += matA[(rowA+1) * N + iter] * matB[iter * N + (colB+1)];

        // printf("%d ", sum);
      }

      // compute output indices
      int rowC = rowA>>1;
      int colC = colB>>1;
      int indexC = rowC * (N>>1) + colC;
    //   printf("%d ", indexC);
      output[indexC] = sum;
    }
  }

  printArray(N/2, output);

}


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

  // printArray(N/2, output);

}      // printf("%d ", sum);
      }

  

void ini_rand(int N, int *arr){
    for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			arr[i*N + j] = (rand() % 10) +1;
		}
	}
    // printArray(N, arr);
}

void ini_zero(int N, int *arr){
    for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			arr[i*N + j] = 0;
		}
	}
}

int main(){

    int N = 16;
    int A[N][N];
    int array1[N][N];
    int array2[N][N];
    int output[N/2][N/2];

	time_t t1; 
    	srand (128); 
	
	

    ini_rand(N, array1);
    ini_rand(N, array2);
    ini_zero(N/2, output);

	singleThread(N, array1, array2, output);

    
    

    //Sanity check

    // int output2[N/2][N/2];
    // ini_zero(N/2, output2);
    // multiThread(N,0,0, array1, array2, output2);
    // multiThread(N,0,N/2, array1, array2, output2);
    // multiThread(N,N/2,0, array1, array2, output2);
    // multiThread(N,N/2,N/2, array1, array2, output2);


    // Thread
    // struct Arguments arg1 = {N, 0, 0, array1, array2, output2};
    // struct Arguments * arg1_ptr = &arg1;
    // printf("%d \n", arg1.N);
    // pthread_t thread1, thread2, thread3, thread4;
    // pthread_create( &thread1, NULL, multiThread_struct, (void*) arg1_ptr);
    // pthread_join(thread1, NULL);

    int output2[N/2][N/2];
    ini_zero(N/2, output2);
    int N_THREADS = 4; 
    pthread_t threads[N_THREADS];
    struct Arguments args[N_THREADS];
    struct Arguments * args_ptrs[N_THREADS];

    for(int i =0; i<N_THREADS; i++){
      args[i].N = N;
      args[i].row_start = (i/2)*N/2;
      args[i].col_start = (i%2)*N/2;
      args[i].matA = array1;
      args[i].matB = array2;
      args[i].output = output2;
    
      args_ptrs[i] = &args[i];
    }

    for(int i =0; i<N_THREADS; i++){
      pthread_create( &threads[i], NULL, multiThread_struct, (void*) args_ptrs[i]);
    }

    for(int i =0; i<N_THREADS; i++){
      pthread_join(threads[i], NULL);
    }

    printArray(N/2, output2);

 	return 0;
}
