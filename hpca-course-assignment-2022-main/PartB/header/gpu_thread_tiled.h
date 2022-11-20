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
__global__ void mat_mul_tiled(int N, int tile, int *matA, int *matB, int *output){
    int rowC = (blockIdx.y * blockDim.y) + threadIdx.y;
    int colC = (blockIdx.x * blockDim.x) + threadIdx.x;
    int indexC = rowC * (N>>1) + colC;

    // if (rowC, colC) = (3,2) then this element is made from 
    //C[3][2] = (A[6,:] + A[7,:]) * (B[:,4] + B[:,5])
    int matA_ptr = 2*rowC;
    int matB_ptr = 2*colC;
    // if (thread_id >= N*N/4)
    //     return;

    int temp = 0;
    for (int i =0; i<N; i+=tile){
        temp += (matA[matA_ptr] + matA[matA_ptr + N])*(matB[matB_ptr] + matB[matB_ptr + 1]);
        matA_ptr += tile; 
        matB_ptr += tile;
    }

    // wait for all threads to finish
    __syncthreads();

    // sum results from these b threds which fills the entry output[rowC][colC]
    // serial execution of b threads 
    for (int b = 0; b < tile; b++){
        // Only one thread from b threads will be allowed &
        // make sure all one thread access output array
        if (threadIdx.x == b && threadIdx.y == b)   
            output[indexC]+= temp;
        
        __syncthreads() // all other threads waits here
    }
}

// Fill in this function
void gpuThreadTiled(int N, int *matA, int *matB, int *output)
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
    int TILE = 4;

    dim3 threadsPerBlock(TILE, TILE);
    dim3 numBlocks(0.5*N/TILE, 0.5*N/TILE);

    mat_mul_tiled <<< threadsPerBlock, numBlocks >>> (N, TILE, matA_gpu, matB_gpu, output_gpu);


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
