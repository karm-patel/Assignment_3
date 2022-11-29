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


__global__ void mat_mul_naive(int N, int *matA, int *matB, int *output){
    /*
    Naive implementation: 
    - We create (N/2)*(N/2) threads
    - Each thread calculate one element of output.
    - Henc, each thread multiply one row of A with one column of B.
    */

    // Thread blocks are in 1D grid and threads in threa block are also in 1D
    int thread_id = (blockIdx.x * blockDim.x) + threadIdx.x;

    if (thread_id >= N*N/4)
        return;

    // multiplied by 2, since A & B has doubled sized than output.
    int rowA = 2*(2*thread_id/N);
    int colB = 2*thread_id%N;
    int j = colB;

    // Current thread will calculate output[thread_id], so it multiply A[rowA] * B[colB]
    for (int i = rowA*N; i < (rowA+1)*N; i++){
        output[thread_id] += (matA[i] + matA[i + N])*(matB[j] + matB[j+1]);
        j+=N;
    }
}

__global__ void mat_mul_tiled(int N, int tile, int *matA, int *matB, int *output){
    /*
    Tiled implementation:
    - We create (N/2)*(N/2) threads

    */
    int tx, ty;
    tx = threadIdx.x;
    ty = threadIdx.y;

    // printf("Blockidx: %d %d Blockdim: %d %d Threads %d, %d\n", blockIdx.x, blockIdx.y, blockDim.x, blockDim.y, tx,ty);
    int rowC = (blockIdx.y * blockDim.y) + ty;
    int colC = (blockIdx.x * blockDim.x) + tx;
    int indexC = rowC * (N>>1) + colC;

    extern __shared__ int blockAB[]; // contains two array BlockA[tile*tile] & BlockB[tile*tile]
    int * blockA = blockAB; // extract blockA
    int * blockB = blockAB + tile*tile; //extract blockB

    if (indexC >= 0.25*(N * N)) return;

    // if (rowC, colC) = (3,2) then this element is made from 
    //C[3][2] = (7th row of A + 8th row of A) * (5th column of B + 6th column of B)
    int matA_ptr = 2*rowC*N + tx;
    int matB_ptr = 2*colC + ty*N;

    int sum = 0;
    for (int i =0; i<N; i+=tile){
        
        // Make small matrix of size (tile x tile) by summing adjecent rows and columns
        // We will not use "tx*tile + ty" because Thread(1, 3) located at 3rd row and 1st column in the blockA
        blockA[ty*tile + tx] = matA[matA_ptr] + matA[matA_ptr + N]; // row N + row N+1
        blockB[ty*tile + tx] = matB[matB_ptr] + matB[matB_ptr + 1]; // col N + col N+1

        // wait for all threads to fill blockA and blockB completely
        __syncthreads();

        // fill blockC = blockA * blockB 
        // current thread will multiply tx row of A with ty column of B ~ naive kernel
        // where, sum = blockC[tx][ty]
        for (int a=0; a<tile; a++){
            sum += blockA[ty*tile + a]*blockB[a*tile + tx];
        }
        
        matA_ptr += tile; // row vise increment
        matB_ptr += N*tile; // column vise increment

        __syncthreads();
    }
    output[indexC] = sum;
}

// Fill in this function
void gpuThread(int N, int *matA, int *matB, int *output)
{

    ini_zero(N>>1, output);
    cudaSetDevice(4);
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
    dim3 numBlocks((N>>1)/TILE, (N>>1)/TILE);

    switch (kernel){

        case 0: // Naive implementation
            int N_THREADS_PER_BLOCK, N_BLOCKS;
            N_THREADS_PER_BLOCK = 1 << 10;
            N_BLOCKS = (N*N/4 + N_THREADS_PER_BLOCK - 1) / N_THREADS_PER_BLOCK;
            mat_mul_naive <<< N_BLOCKS, N_THREADS_PER_BLOCK >>> (N, matA_gpu, matB_gpu, output_gpu);
        break;

        case 1: // Tiled implementation
            mat_mul_tiled <<<numBlocks, threadsPerBlock, 2*TILE*TILE*sizeof(int)>>> (N, TILE, matA_gpu, matB_gpu, output_gpu);
            break;

        default:
            printf("Invalid kernel\n");
            exit(1);

    }  

    // This will acts as barries for all threads
    cudaMemcpy(output, output_gpu, bytes/4, cudaMemcpyDeviceToHost);
    
    //Free memory on gpu
    cudaFree(matA_gpu);
    cudaFree(matB_gpu);
    cudaFree(output_gpu);

    std::cout << "Multiplied\n";
    }
