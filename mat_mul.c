#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>


int N = 4;
float A[4][4];
float array1[4][4];
float array2[4][4];

void printArray(float A[N][N],int N){
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			printf("%f ",A[i][j] );
		}
		printf("\n");
	}
}

void printArray_ptr(float *a,int N){
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			printf("%f ",a[i*N + j]);
		}
		printf("\n");
	}
}

void multiplication(){
for(int i=0; i<N; i++)
	{	
		for(int j=0; j<N; j++){
 			
            A[i][j]= 0;
 			for (int k=0; k < N; k++){
                    A[i][j] += array1[i][k]*array2[k][j];
 				}
 			}
 							
 	} 
}

void multiplication_ptr_ijk(float *a, float *b, float *c){
for(int i=0; i<N; i++)
	{	
		for(int j=0; j<N; j++){
 			
 			for (int k=0; k < N; k++){
                    c[i*N + j] = a[i*N + k]*b[k*N + j];
                    // printf("%d %d",a[i*N + k], b[k + j*N]); Why this is not priniting desired numbers?
 				}
 			}
 							
 	} 
printf("\nPTR ANS:\n");
printArray_ptr(c,N);
}

void multiplication_ptr_kij(float *a, float *b, float *c){
for(int k=0; k<N; k++)
	{	
		for(int i=0; i<N; i++){
 			
 			for (int j=0; j < N; j++){
                    c[i*N + j] += a[i*N + k]*b[k*N + j];
                    printf("c[%d][%d]+= a[%d][%d]*b[%d][%d] ",i,j,i,k,k,j);
                    // printf("%d %d",a[i*N + k], b[k + j*N]); Why this is not priniting desired numbers?
 				}
                printf("\n");
 			}
 							
 	} 
printf("\nPTR ANS:\n");
printArray_ptr(c,N);
}


int main(){
	
	time_t t1; 
    	srand (128); 
	
	
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			array1[i][j] = (rand() % 10) +1;
		}
	}
	
	
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			array2[i][j] = (rand() % 10) + 1;
		}
	}
	
float output[4][4];

for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
        output[i][j] = 0;
    }
}

	
	multiplication();

printf("\nARRAY 1:\n");
 printArray(array1,N);

 printf("\nARRAY 2:\n");
 printArray(array2,N);

multiplication_ptr_kij(array1, array2, output);

 printf("\nANS:\n");
 printArray(A,N);
 	
// printArray_ptr(output, N);

 	return 0;
}

//testing


