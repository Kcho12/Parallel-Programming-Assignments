/* Matrix normalization.
 compile with "nvcc matrixNormCuda.cu -o matrixNormCuda""
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>
 #include <sys/time.h>
 #include <math.h>
 
 /* Program Parameters */
 #define MAXN 6000  /* Matrix size */
 int N = 6000;
 dim3 dG_numBlocks; //dG - dimension and size of grid in blocks
 dim3 dB_numThreads; //dB - dimension and size of blocks in threads
 
 /* Matrices */
 float A[MAXN * MAXN], B[MAXN * MAXN];
 float *A_d, *B_d; // device data equivalent to A and B
 
 /* Set the program parameters from the command-line arguments */
 void parameters(int argc, char **argv) {//added parameters from guass_serial.c so we can define the function dimensions
     int seed = 0;  /* Random seed */
     // char uid[32]; /*User name */
     dG_numBlocks = 32; //default values if we just want to quickly test matrixNorm with just ./matrixNorm
     dB_numThreads = 64;
 
     /* Read command-line arguments */
     srand((unsigned)time(NULL));  /* Randomize */
 
     if (argc == 5) {
         seed = atoi(argv[2]);
         srand(seed);
         dG_numBlocks=atoi(argv[3]);
         dB_numThreads=atoi(argv[4]);
         printf("Random seed = %i\n", seed);
         printf("Number of Blocks = %i\n", dG_numBlocks);
         printf("Number of Threads per block = %i\n", dB_numThreads);
     }
     if (argc == 4) { // ./matrixNormCuda <matrix_dimension> <number of Blocks> <number of threads per block>
        dG_numBlocks=atoi(argv[2]);
        dB_numThreads=atoi(argv[3]);
        printf("Number of Blocks = %i\n", dG_numBlocks);
        printf("Number of Threads per block = %i\n", dB_numThreads);
    }
     if (argc == 3) {// ./matrixNormCuda <matrix_dimension> [random seed] 
        seed = atoi(argv[2]);
        srand(seed);
        printf("Random seed = %i\n", seed);
    }
     if (argc >= 2) {
         N = atoi(argv[1]);
         if (N < 1 || N > MAXN) {
             printf("N = %i is out of range.\n", N);
             exit(0);
         }
     }
     if (argc >= 1) {
        //do ./matrixNormCuda 6000
	}
     else {
         printf("Usage: %s <matrix_dimension> [random seed] <number of Blocks> <number of threads per block>\n",
                 argv[0]);    
         exit(0);
     }
 }
     
 /* Initialize A and B*/
 void initialize_inputs() {
     int row, col;
     
     //srand((unsigned)time(NULL));
     for (row = 0; row < N; row++) {
         for (col = 0; col < N; col++) {
             A[row * N + col] = (float)rand() / 32768.0;//we need to copy this from host to the device
             B[row * N + col] = 0.0;
         }
     }
     
 }

/* Print input matrices */
void print_inputs() {
    int row, col;

    if (N < 100) {
        printf("\nA =\n\t");
        for (row = 0; row < N; row++) {
            for (col = 0; col < N; col++) {
                printf("%5.2f%s", A[row*N+col], (col < N-1) ? ", " : ";\n\t");
            }
        }
    }
}      

 //Display output for correctness comparison
 void print_B() {
     int row, col;
 
     if (N < 10) {
         printf("\nB = [");
         for (row = 0; row < N; row++) {
             for (col = 0; col < N; col++) {
                 printf("%5.2f%s", B[row * N + col], (col < N-1) ? ", " : "]\n");
             }
         }
     }
 }
 
 /* Kernel function */
 __global__ void matrixNorm(float *A_d, float *B_d, int N) {
    int row;
    float mu, sigma; // Mean and Standard Deviation
 
    int col = blockIdx.x * blockDim.x + threadIdx.x;//Treat col as the same as idx
     
     if (col < N) {//col is now incremented based on the blockIdx and threadIdx
         mu = 0.0;
         for (row=0; row < N; row++)
             mu += A_d[row * N + col]; //expression must have a pointer-to-object type. Since we are incrementing by every column automatically, we can just traverse the rows by multiplying by N.
         mu /= (float) N;
        __syncthreads();//wait for all cuda threads to get the mean
         
 
         sigma = 0.0;
         for (row=0; row < N; row++)
             sigma += powf(A_d[row * N + col] - mu, 2.0);
         sigma /= (float) N;
         __syncthreads();//wait for all cuda threads to get the stdeviation
 
         sigma = sqrt(sigma);
         for (row=0; row < N; row++) {
             if (sigma == 0.0)
                 B_d[row * N + col] = 0.0;
             else
                 B_d[row * N + col] = (A_d[row * N + col] - mu) / sigma;
         }
     }
 }
 
 
 int main(int argc, char **argv) {
     /* Timing variables */
     struct timeval start, stop;  /* Elapsed times using gettimeofday() */
     struct timezone tzdummy;
     unsigned long long runtime;
     float elapsed=0;/* Cuda timing elements*/
     cudaEvent_t cstart, cstop;
     cudaEventCreate(&cstart);
     cudaEventCreate(&cstop);
 
     /* Process program parameters */ //Imported from guass_serial.c
     parameters(argc, argv);
     
     /* Initialize A and B */
     initialize_inputs();

     //print_inputs();
 
     /* Start Clock */
     printf("\n---------------------------------------------\n");
     printf("Matrix size N = %d", N);
     printf("\nStarting clock.\n\n");
     gettimeofday(&start, &tzdummy);
     cudaEventRecord(cstart, 0);
     
    /*Data movement from Host to Device*/
     //cudaMalloc(void ** pointer, size_t nbytes)
     cudaMalloc((void **) &A_d, sizeof(float)*N*N);//These mallocs have insane overhead. They add 1000ms to the runtime.
     cudaMalloc((void **) &B_d, sizeof(float)*N*N);
 
     //cudaMemcpy(void *dst, void *src, size_t nbytes, enum cudaMemcpyKind direction);
     cudaMemcpy(A_d, A, sizeof(float)*N*N, cudaMemcpyHostToDevice); //copy A matrix from host to A_d matrix in devices
     
     /* Matrix Normalization */
     printf("Computing with Cuda.\n");
     matrixNorm<<<dG_numBlocks, dB_numThreads>>>(A_d, B_d, N);
     cudaDeviceSynchronize(); //Fixes the timing for GPU. Makes sure all cuda threads are done before stoping the clock
   
     /* Stop Clock */
     gettimeofday(&stop, &tzdummy);
     runtime = (unsigned long long)(stop.tv_sec - start.tv_sec) * 1000000 + (stop.tv_usec - start.tv_usec);

    cudaEventRecord(cstop, 0);
    cudaEventSynchronize (cstop);

    cudaEventElapsedTime(&elapsed, cstart, cstop);
 
     /* Display output */
     cudaMemcpy(B, (B_d), sizeof(float)*N*N, cudaMemcpyDeviceToHost); //send device B back to Host B for display
     print_B();
     
     cudaFree(A_d);
     cudaFree(B_d);
     
     /* Display timing results */
     printf("Runtime = %g ms.\n", (float)runtime/(float)1000);
     printf("\nTime in gpu was %.2f ms\n", elapsed); //Time in GPU is more important for how efficient our algorithm is than total runtime.
     printf("\nStopped clock.");
     printf("\n---------------------------------------------\n");
     
     exit(0);
 }