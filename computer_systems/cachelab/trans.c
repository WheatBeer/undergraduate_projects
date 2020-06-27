/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N]){

    int i, j, t, s; //t: row block, s: column block
	int tmp[8];
    int block_size; 

    //(M = 32, N = 32) or (M = 61, N = 67)
    if(M == 32 || M == 61){ //modify the code of multiplication at "http://csapp.cs.cmu.edu/public/waside/waside-blocking.pdf" 
        //Each block_size is optimized.
        if(M == 32) block_size = 8;
        else block_size = 16; 

		for(s = 0; s < M; s += block_size){
			for(t = 0; t < N; t += block_size){
                //To transpose the (M = 61 x N = 67) matrix, I put "i < N" and "j < N" conditions in each for-loop. 
				for(i = t; (i < t + block_size) && i < N; i ++){ 
					for(j = s; (j < s + block_size) && j < M; j ++){
						if(i != j){
							B[j][i] = A[i][j];
						}
						 else{
							tmp[1] = A[i][j];
							tmp[0] = i;
						}
					}
					if(t == s){
						B[tmp[0]][tmp[0]] = tmp[1];
					}
				}		
			}
		}
	}
    /*
    * (M = 64, N = 64)
    * I wrote miss and hit on the code. 
    */
    else{
        //block_size = 8 (bcause of cache size)
        //during each of the outest for-loop, deal with 8x8 matrix in 64x64 matrix
		for(i = 0; i < N; i += 8){
			for(j = 0; j < M; j += 8){
                //load and store just 4x8 matrix data in 8x8 matrix
				for(t = 0; t < 4; t++){
                    tmp[0] = A[i+t][j+0]; //miss
                    tmp[1] = A[i+t][j+1]; //hit
                    tmp[2] = A[i+t][j+2]; //hit
                    tmp[3] = A[i+t][j+3]; //hit
                    tmp[4] = A[i+t][j+4]; //miss
                    tmp[5] = A[i+t][j+5]; //hit
                    tmp[6] = A[i+t][j+6]; //hit
                    tmp[7] = A[i+t][j+7]; //hit

                    //t=0: all miss, t=1,2,3: all hit
                    B[j+0][i+t] = tmp[0]; //done
                    B[j+0][i+t+4] = tmp[5];	
                    B[j+1][i+t] = tmp[1]; //done	
                    B[j+1][i+t+4] = tmp[6];	
                    B[j+2][i+t] = tmp[2]; //done
                    B[j+2][i+t+4] = tmp[7];	
                    B[j+3][i+t] = tmp[3]; //done
                    B[j+3][i+t+4] = tmp[4];
                    //B is only half completed
                    //B[j+0][i+4], B[j+3][i+5], B[j+3][i+6], B[j+3][i+7] will be used  
                }

                //To complete the 4x8 part of B and remain later-use part, load other two 4x4 part of A
                tmp[0] = A[i+4][j+4]; 
                tmp[1] = A[i+5][j+4];
                tmp[2] = A[i+6][j+4];
                tmp[3] = A[i+7][j+4];
                tmp[4] = A[i+4][j+3];
                tmp[5] = A[i+5][j+3];
                tmp[6] = A[i+6][j+3];
                tmp[7] = A[i+7][j+3]; //all miss


                B[j+4][i+0] = B[j+3][i+4]; //miss & hit in order equation
                B[j+4][i+4] = tmp[0]; //miss
                B[j+3][i+4] = tmp[4]; //hit
                B[j+4][i+1] = B[j+3][i+5]; //double hit 
                B[j+4][i+5] = tmp[1]; //hit
                B[j+3][i+5] = tmp[5]; //hit
                B[j+4][i+2] = B[j+3][i+6]; //double hit
                B[j+4][i+6] = tmp[2]; //hit
                B[j+3][i+6] = tmp[6]; //hit
                B[j+4][i+3] = B[j+3][i+7]; //double hit
                B[j+4][i+7] = tmp[3]; //hit
                B[j+3][i+7] = tmp[7]; //hit


                //fill the remaining space of 8x8 B
                for(t=0;t<3;t++){
                    //all hit
                    tmp[0] = A[i+4][j+5+t];
                    tmp[1] = A[i+5][j+5+t];
                    tmp[2] = A[i+6][j+5+t];
                    tmp[3] = A[i+7][j+5+t];
                    tmp[4] = A[i+4][j+t];
                    tmp[5] = A[i+5][j+t];
                    tmp[6] = A[i+6][j+t];
                    tmp[7] = A[i+7][j+t];

                    B[j+5+t][i+0] = B[j+t][i+4]; //miss & hit
                    B[j+5+t][i+4] = tmp[0]; //miss
                    B[j+t][i+4] = tmp[4]; //hit
                    B[j+5+t][i+1] = B[j+t][i+5]; //double hit
                    B[j+5+t][i+5] = tmp[1]; //hit
                    B[j+t][i+5] = tmp[5]; //hit
                    B[j+5+t][i+2] = B[j+t][i+6]; //double hit
                    B[j+5+t][i+6] = tmp[2]; //hit
                    B[j+t][i+6] = tmp[6]; //hit
                    B[j+5+t][i+3] = B[j+t][i+7]; //double hit
                    B[j+5+t][i+7] = tmp[3]; //hit
                    B[j+t][i+7] = tmp[7]; //hit
                }

        	}
                
		}
    }
}
    
/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    //registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

