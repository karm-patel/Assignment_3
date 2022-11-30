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

__global__ void mat_mul_tiled(int N, int tile, int *matA, int *matB, int *output){
    int tx, ty;
    tx = threadIdx.x;
    ty = threadIdx.y;
    // printf("%d %d\n", tx, ty);
    // printf("Blockidx: %d %d Blockdim: %d %d Threads %d, %d\n", blockIdx.x, blockIdx.y, blockDim.x, blockDim.y, tx,ty);
    int rowC = (blockIdx.y * blockDim.y) + ty;
    int colC = (blockIdx.x * blockDim.x) + tx;
    int indexC = rowC * (N>>1) + colC;

    // __shared__ int blockA[16][16];
    // __shared__ int blockB[16][16];

    extern __shared__ int blockAB[];

    int * blockA = blockAB;
    int * blockB = blockAB + tile*tile;

    if (indexC >= 0.5*(N * N)) return;

    // if (rowC, colC) = (3,2) then this element is made from 
    //C[3][2] = (A[6,:] + A[7,:]) * (B[:,4] + B[:,5])
    int matA_ptr = 2*rowC*N + tx;
    int matB_ptr = 2*colC + ty*N;

    if (indexC == 0)
        printf("%d: %d %d - - %d %d (%d, %d)\n",indexC, rowC, colC, matA_ptr, matB_ptr, tx, ty);
    // if (thread_id >= N*N/4)
    //     return;

    int temp = 0;
    for (int i =0; i<N; i+=tile){
        
        // Make small matrix of tile x tile by summing adjecent rows and columns
        blockA[ty*tile + tx] = matA[matA_ptr] + matA[matA_ptr + N]; // row N + row N+1
        blockB[ty*tile + tx] = matB[matB_ptr] + matB[matB_ptr + 1]; // col N + col N+1

        // wait for all threads to fill blockA and blockB completely
        __syncthreads();

        if (indexC == 0)
        {
        printf("\nA = \n");
        for (int a =0; a<tile; a++){
            for (int b =0; b<tile; b++){
                printf("blockA[%d][%d] = %d ",a, b, blockA[a*tile + b]);
            }
            printf("\n");  
        }
        printf("\nB: \n");
        for (int a =0; a<tile; a++){
            for (int b =0; b<tile; b++){
                printf("blockB[%d][%d] = %d ",a, b, blockB[a*tile + b]);
            }
            printf("\n");  
        }
        }

        // fill blockC = blockA * blockB 
        // current thread will multiply tx th row of A with ty column of B 
        // temp = blockC[tx][ty]
        for (int a=0; a<tile; a++){
            temp += blockA[ty*tile + a]*blockB[a*tile + tx];
            // if (indexC == 0)
            //     printf("%d * %d + ",blockA[tx][a],blockB[a][ty]);

        }
        
        matA_ptr += tile;
        matB_ptr += N*tile;

        __syncthreads();
    }

    output[indexC] = temp;

}

// Fill in this function
void gpuThread(int N, int *matA, int *matB, int *output)
{

    ini_zero(N/2, output);
    cudaSetDevice(1);
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


    int kernel = 1;
    int TILE = 32;
    dim3 threadsPerBlock(TILE, TILE);
    dim3 numBlocks(0.5*N/TILE, 0.5*N/TILE);

    switch (kernel){

        case 0: // Naive implementation
            int N_THREADS_PER_BLOCK, N_BLOCKS;
            N_THREADS_PER_BLOCK = 1 << 10;
            N_BLOCKS = (N*N/4 + N_THREADS_PER_BLOCK - 1) / N_THREADS_PER_BLOCK;
            mat_mul <<< N_BLOCKS, N_THREADS_PER_BLOCK >>> (N, matA_gpu, matB_gpu, output_gpu);
        break;

        case 1: // Tiled implementation
            mat_mul_tiled <<<numBlocks, threadsPerBlock, 2*TILE*TILE*sizeof(int)>>> (N, TILE, matA_gpu, matB_gpu, output_gpu);
            break;

        default:
            printf("Invalid kernel\n");
            exit(1);

    }  

    cudaMemcpy(output, output_gpu, bytes/4, cudaMemcpyDeviceToHost);
    
    // printf("freed");
    //Free memory on gpu
    cudaFree(matA_gpu);
    cudaFree(matB_gpu);
    cudaFree(output_gpu);

    // printArray(N/2, output);
    std::cout << "Multiplied\n";
    
    }
