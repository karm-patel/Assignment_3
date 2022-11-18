// Create other necessary functions here

void ini_zero(int N, int *arr){
    for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			arr[i*N + j] = 0;
		}
	}
}

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

// blockIdx: stores index of thread block from 1D, 2D, or 3D block grids
// blockDim: the size of thread block 
// threadIdx: vector which contains thread id
__global__ void mat_mul(int N, int *matA, int *matB, int *output){
    int thread_id = (blockIdx.x * blockDim.x) + threadIdx.x;

    if (thread_id >= N*N/4)
        return;

    int rowA = 2*(2*thread_id/N);
    int colB = 2*thread_id%N;
    int j = colB;

    // if (thread_id == 0){
    //     printf("First thread - %d ",  thread_id);
    //     for (int i = rowA*N; i < (rowA+1)*N; i++){
    //         output[thread_id] += matA[i]*matB[j];
    //         j+=N;
    //     }
    // }
    // else{
    // printf("R:%d, C:%d\n", rowA, colB);
    for (int i = rowA*N; i < (rowA+1)*N; i++){
        output[thread_id] += (matA[i] + matA[i + N])*(matB[j] + matB[j+1]);
        // printf("(%d + %d)*(%d + %d) + ", matA[i], matA[i+N], matB[j], matB[j+1]);
        j+=N;
    }
    // printf("\n");
}
// }

// Fill in this function
void gpuThread(int N, int *matA, int *matB, int *output)
{

    ini_zero(N/2, output);
    cudaSetDevice(2);
    size_t bytes = sizeof(int) * N * N;

    // Assign memory from GPU to gpu arrays
    int *matA_gpu, *matB_gpu, *output_gpu;
    cudaMalloc(&matA_gpu, bytes);
    cudaMalloc(&matB_gpu, bytes);
    cudaMalloc(&output_gpu, bytes/4);

    // 
    cudaMemcpy(matA_gpu, matA, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(matB_gpu, matB, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(output_gpu, output, bytes/4, cudaMemcpyHostToDevice);


    int N_THREADS_PER_BLOCK, N_BLOCKS;

    N_THREADS_PER_BLOCK = 1 << 10;
    N_BLOCKS = (N*N/4 + N_THREADS_PER_BLOCK - 1) / N_THREADS_PER_BLOCK;

    mat_mul <<< N_BLOCKS, N_THREADS_PER_BLOCK >>> (N, matA_gpu, matB_gpu, output_gpu);


    // cudaMemcpy(matA, matA_gpu, bytes, cudaMemcpyDeviceToHost);
    // cudaMemcpy(matB, matB_gpu,  bytes, cudaMemcpyDeviceToHost);
    cudaMemcpy(output, output_gpu, bytes/4, cudaMemcpyDeviceToHost);
    
    // printf("freed");
    //Free memory on gpu
    cudaFree(matA_gpu);
    cudaFree(matB_gpu);
    cudaFree(output_gpu);

    // printArray(N/2, output);
    std::cout << "Multiplied\n";
    
    }
