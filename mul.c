/*
 * mul.c - Matrix multiply C = A * B
 *
 * Each multiply function must have a prototype of the form:
 * void mul(int M, int N, int A[N][M], int B[M][N], int C[N][N]);
 *
 * A multiply function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_mul(int M, int N, int A[N][M], int B[M][N], int C[N][N]);

/*
 * mul_submit - This is the solution multiply function that you will be
 *     graded on for Part C of the assignment. Do not change the
 *     description string "Multiply submission", as the driver searches
 *     for that string to identify the multiply function to be graded.
 *     C starts out holding garbage, so every element of C must be written.
 */
char mul_submit_desc[] = "Multiply submission";
void mul_submit(int M, int N, int A[N][M], int B[M][N], int C[N][N])
{
    int i, j, k;
    int c0, c1, c2, c3, c4, c5, c6, c7, a;

    if (M == 32 && N == 32) {
        for (j = 0; j < 32; j += 8) {
            for (k = 0; k < 32; k += 8) {
                for (i = 0; i < 32; i++) {
                    if (k == 0) {
                        c0 = 0;
                        c1 = 0;
                        c2 = 0;
                        c3 = 0;
                        c4 = 0;
                        c5 = 0;
                        c6 = 0;
                        c7 = 0;
                    } else {
                        c0 = C[i][j];
                        c1 = C[i][j + 1];
                        c2 = C[i][j + 2];
                        c3 = C[i][j + 3];
                        c4 = C[i][j + 4];
                        c5 = C[i][j + 5];
                        c6 = C[i][j + 6];
                        c7 = C[i][j + 7];
                    }
                    /* Reuse each A element across one B cache block. */
                    a = A[i][k + 0];
                    c0 += a * B[k + 0][j];
                    c1 += a * B[k + 0][j + 1];
                    c2 += a * B[k + 0][j + 2];
                    c3 += a * B[k + 0][j + 3];
                    c4 += a * B[k + 0][j + 4];
                    c5 += a * B[k + 0][j + 5];
                    c6 += a * B[k + 0][j + 6];
                    c7 += a * B[k + 0][j + 7];
                    a = A[i][k + 1];
                    c0 += a * B[k + 1][j];
                    c1 += a * B[k + 1][j + 1];
                    c2 += a * B[k + 1][j + 2];
                    c3 += a * B[k + 1][j + 3];
                    c4 += a * B[k + 1][j + 4];
                    c5 += a * B[k + 1][j + 5];
                    c6 += a * B[k + 1][j + 6];
                    c7 += a * B[k + 1][j + 7];
                    a = A[i][k + 2];
                    c0 += a * B[k + 2][j];
                    c1 += a * B[k + 2][j + 1];
                    c2 += a * B[k + 2][j + 2];
                    c3 += a * B[k + 2][j + 3];
                    c4 += a * B[k + 2][j + 4];
                    c5 += a * B[k + 2][j + 5];
                    c6 += a * B[k + 2][j + 6];
                    c7 += a * B[k + 2][j + 7];
                    a = A[i][k + 3];
                    c0 += a * B[k + 3][j];
                    c1 += a * B[k + 3][j + 1];
                    c2 += a * B[k + 3][j + 2];
                    c3 += a * B[k + 3][j + 3];
                    c4 += a * B[k + 3][j + 4];
                    c5 += a * B[k + 3][j + 5];
                    c6 += a * B[k + 3][j + 6];
                    c7 += a * B[k + 3][j + 7];
                    a = A[i][k + 4];
                    c0 += a * B[k + 4][j];
                    c1 += a * B[k + 4][j + 1];
                    c2 += a * B[k + 4][j + 2];
                    c3 += a * B[k + 4][j + 3];
                    c4 += a * B[k + 4][j + 4];
                    c5 += a * B[k + 4][j + 5];
                    c6 += a * B[k + 4][j + 6];
                    c7 += a * B[k + 4][j + 7];
                    a = A[i][k + 5];
                    c0 += a * B[k + 5][j];
                    c1 += a * B[k + 5][j + 1];
                    c2 += a * B[k + 5][j + 2];
                    c3 += a * B[k + 5][j + 3];
                    c4 += a * B[k + 5][j + 4];
                    c5 += a * B[k + 5][j + 5];
                    c6 += a * B[k + 5][j + 6];
                    c7 += a * B[k + 5][j + 7];
                    a = A[i][k + 6];
                    c0 += a * B[k + 6][j];
                    c1 += a * B[k + 6][j + 1];
                    c2 += a * B[k + 6][j + 2];
                    c3 += a * B[k + 6][j + 3];
                    c4 += a * B[k + 6][j + 4];
                    c5 += a * B[k + 6][j + 5];
                    c6 += a * B[k + 6][j + 6];
                    c7 += a * B[k + 6][j + 7];
                    a = A[i][k + 7];
                    c0 += a * B[k + 7][j];
                    c1 += a * B[k + 7][j + 1];
                    c2 += a * B[k + 7][j + 2];
                    c3 += a * B[k + 7][j + 3];
                    c4 += a * B[k + 7][j + 4];
                    c5 += a * B[k + 7][j + 5];
                    c6 += a * B[k + 7][j + 6];
                    c7 += a * B[k + 7][j + 7];
                    C[i][j] = c0;
                    C[i][j + 1] = c1;
                    C[i][j + 2] = c2;
                    C[i][j + 3] = c3;
                    C[i][j + 4] = c4;
                    C[i][j + 5] = c5;
                    C[i][j + 6] = c6;
                    C[i][j + 7] = c7;
                }
            }
        }
    } else {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                c0 = 0;
                for (k = 0; k < M; k++)
                    c0 += A[i][k] * B[k][j];
                C[i][j] = c0;
            }
        }
    }
}

/*
 * mul - A simple baseline multiply function, not optimized for the cache.
 */
char mul_desc[] = "Simple i-j-k multiply";
void mul(int M, int N, int A[N][M], int B[M][N], int C[N][N])
{
    int i, j, k, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            tmp = 0;
            for (k = 0; k < M; k++)
                tmp += A[i][k] * B[k][j];
            C[i][j] = tmp;
        }
    }
}

/*
 * registerFunctions - This function registers your multiply functions
 *     with the driver. At runtime, the driver will evaluate each of the
 *     registered functions and summarize their performance. This is a
 *     handy way to experiment with different multiply strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerMulFunction(mul_submit, mul_submit_desc);

    /* Register any additional multiply functions */
    registerMulFunction(mul, mul_desc);
}

/*
 * is_mul - This helper function checks if C is the product of A and B.
 *     You can check the correctness of your multiply by calling it
 *     before returning from the multiply function.
 */
int is_mul(int M, int N, int A[N][M], int B[M][N], int C[N][N])
{
    int i, j, k, num;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            num = 0;
            for (k = 0; k < M; k++)
                num += A[i][k] * B[k][j];
            if (num != C[i][j])
                return 0;
        }
    }
    return 1;
}
