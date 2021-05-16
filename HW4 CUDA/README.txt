I used CC-CentOS7-CUDA10-2003 image for my instance on node P100 on Chameleon. 
I've also included a modified serial matrixNorm.c for parameter changing, and correctness tests.

For matrixNorm.c
1. Compile with "gcc matrixNorm.c -lm -o matrixNorm" 
2. test output with ./matrixNorm (if you just want to test N = 6000)
3. test specific output with ./matrixNorm <matrix_dimension> [random seed]

For matrixNormCuda.cu
1. Compile with "nvcc matrixNormCuda.cu -o matrixNormCuda"
2. test specific output with ./matrixNormCuda <matrix_dimension> [random seed]
3. test output with ./matrixNormCuda <matrix_dimension> [random seed] <number of Blocks> <number of threads per block>

<<PERFORMANCE RESULTS FOR SCALING N>>
./matrixNorm 1000 10 -> Runtime = 28.873 ms.
./matrixNorm 1000 10 -> Runtime = 28.802 ms.
./matrixNorm 1000 10 -> Runtime = 27.484 ms.

./matrixNorm 2000 10 -> Runtime = 138.406 ms.
./matrixNorm 2000 10 -> Runtime = 131.281 ms.
./matrixNorm 2000 10 -> Runtime = 123.998 ms.

./matrixNorm 3000 10 -> Runtime = 382.984 ms.
./matrixNorm 3000 10 -> Runtime = 382.884 ms.
./matrixNorm 3000 10 -> Runtime = 418.388 ms.

./matrixNorm 4000 10 -> Runtime = 691.169 ms.
./matrixNorm 4000 10 -> Runtime = 691.966 ms.
./matrixNorm 4000 10 -> Runtime = 698.985 ms.

./matrixNorm 5000 10 -> Runtime = 1238.57 ms.
./matrixNorm 5000 10 -> Runtime = 1215.84 ms.
./matrixNorm 5000 10 -> Runtime = 1215.33 ms.

./matrixNorm 6000 10 -> Runtime = 1766.95 ms.
./matrixNorm 6000 10 -> Runtime = 1755.04 ms.
./matrixNorm 6000 10 -> Runtime = 1746.42 ms.

./matrixNormCuda 1000 10 -> Runtime = 2.883 ms.     Time in gpu was 2.87 ms
./matrixNormCuda 1000 10 -> Runtime = 3.058 ms.     Time in gpu was 3.05 ms
./matrixNormCuda 1000 10 -> Runtime = 2.011 ms.     Time in gpu was 2.00 ms

./matrixNormCuda 2000 10 -> Runtime = 4.783 ms.     Time in gpu was 4.77 ms
./matrixNormCuda 2000 10 -> Runtime = 5.506 ms.     Time in gpu was 5.49 ms
./matrixNormCuda 2000 10 -> Runtime = 4.727 ms.     Time in gpu was 4.71 ms

./matrixNormCuda 3000 10 -> Runtime = 10.348 ms.    Time in gpu was 10.33 ms
./matrixNormCuda 3000 10 -> Runtime = 11.444 ms.    Time in gpu was 11.42 ms
./matrixNormCuda 3000 10 -> Runtime = 9.173 ms.     Time in gpu was 9.15 ms

./matrixNormCuda 4000 10 -> Runtime = 13.787 ms.    Time in gpu was 13.76 ms
./matrixNormCuda 4000 10 -> Runtime = 16.055 ms.    Time in gpu was 16.03 ms
./matrixNormCuda 4000 10 -> Runtime = 13.622 ms.    Time in gpu was 13.60 ms

./matrixNormCuda 5000 10 -> Runtime = 21.141 ms.    Time in gpu was 21.12 ms
./matrixNormCuda 5000 10 -> Runtime = 19.379 ms.    Time in gpu was 19.36 ms
./matrixNormCuda 5000 10 -> Runtime = 22.943 ms.    Time in gpu was 22.92 ms

./matrixNormCuda 6000 10 -> Runtime = 25.748 ms.    Time in gpu was 22.71 ms
./matrixNormCuda 6000 10 -> Runtime = 31.211 ms.    Time in gpu was 31.19 ms
./matrixNormCuda 6000 10 -> Runtime = 25.724 ms.    Time in gpu was 25.70 ms

<<PERFORMANCE RESULTS for N = 6000>>
./matrixNorm 6000 10 -> Runtime = 1748.64 ms.
./matrixNorm 6000 10 -> Runtime = 1747.67 ms.
./matrixNorm 6000 10 -> Runtime = 1753.33 ms.

./matrixNormCuda 6000 10 6000 1 -> Runtime = 70.473 ms.    Time in gpu was 74.41 ms
./matrixNormCuda 6000 10 6000 1 -> Runtime = 75.035 ms.    Time in gpu was 75.01 ms
./matrixNormCuda 6000 10 6000 1 -> Runtime = 76.085 ms.    Time in gpu was 76.03 ms

./matrixNormCuda 6000 10 6000 2 -> Runtime = 51.095 ms.    Time in gpu was 51.07 ms
./matrixNormCuda 6000 10 6000 2 -> Runtime = 50.988 ms.    Time in gpu was 50.97 ms
./matrixNormCuda 6000 10 6000 2 -> Runtime = 48.098 ms.    Time in gpu was 48.08 ms

./matrixNormCuda 6000 10 6000 4 -> Runtime = 32.825 ms.    Time in gpu was 32.80 ms
./matrixNormCuda 6000 10 6000 4 -> Runtime = 32.187 ms.    Time in gpu was 32.17 ms
./matrixNormCuda 6000 10 6000 4 -> Runtime = 35.159 ms.    Time in gpu was 32.14 ms

./matrixNormCuda 6000 10 6000 8 -> Runtime = 28.896 ms.    Time in gpu was 28.88 ms
./matrixNormCuda 6000 10 6000 8 -> Runtime = 29.554 ms.    Time in gpu was 29.53 ms
./matrixNormCuda 6000 10 6000 8 -> Runtime = 29.095 ms.    Time in gpu was 29.07 ms

./matrixNormCuda 6000 10 6000 12 -> Runtime = 27.905 ms.   Time in gpu was 27.88 ms
./matrixNormCuda 6000 10 6000 12 -> Runtime = 27.962 ms.   Time in gpu was 27.94 ms
./matrixNormCuda 6000 10 6000 12 -> Runtime = 27.954 ms.   Time in gpu was 27.93 ms

./matrixNormCuda 6000 10 6000 16 -> Runtime = 27.435 ms.   Time in gpu was 27.41 ms
./matrixNormCuda 6000 10 6000 16 -> Runtime = 27.785 ms.   Time in gpu was 27.76 ms
./matrixNormCuda 6000 10 6000 16 -> Runtime = 27.155 ms.   Time in gpu was 27.13 ms

<<CORRECTNESS TESTS>>
./matrixNorm 5 25    ->
B = [ 0.65,  0.18, -1.22,  1.21, -1.62]
 1.09,  1.08,  0.59, -1.33,  0.16]
-1.38, -1.23,  0.91,  0.55,  1.27]
 0.66,  1.05, -1.21,  0.61, -0.50]
-1.03, -1.09,  0.93, -1.05,  0.69]

./matrixNormCuda 5 25   ->
B = [ 0.65,  0.18, -1.22,  1.21, -1.62]
 1.09,  1.08,  0.59, -1.33,  0.16]
-1.38, -1.23,  0.91,  0.55,  1.27]
 0.66,  1.05, -1.21,  0.61, -0.50]
-1.03, -1.09,  0.93, -1.05,  0.69]

./matrixNorm 8 25    ->
B = [ 1.25,  0.65, -1.79,  1.73, -1.25,  1.67,  1.37,  1.08]
-0.94, -0.06,  0.34, -0.26,  1.85,  0.82,  0.58,  1.43]
 1.38, -1.75,  0.57, -0.35,  0.69,  0.07,  1.38, -1.29]
 0.51,  0.95,  0.18,  0.93,  0.55, -0.99, -0.85,  0.37]
-1.11, -0.88,  0.11, -0.86, -1.29,  0.39, -1.07, -0.48]
-0.66,  0.99,  1.08, -1.55,  0.33,  0.54, -1.29,  0.38]
 0.68,  1.00, -1.47, -0.44, -0.66, -1.32,  0.34, -1.62]
-1.11, -0.91,  0.98,  0.78, -0.21, -1.17, -0.47,  0.14]

./matrixNormCuda 8 25   ->
B = [ 1.25,  0.65, -1.79,  1.73, -1.25,  1.67,  1.37,  1.08]
-0.94, -0.06,  0.34, -0.26,  1.85,  0.82,  0.58,  1.43]
 1.38, -1.75,  0.57, -0.35,  0.69,  0.07,  1.38, -1.29]
 0.51,  0.95,  0.18,  0.93,  0.55, -0.99, -0.85,  0.37]
-1.11, -0.88,  0.11, -0.86, -1.29,  0.39, -1.07, -0.48]
-0.66,  0.99,  1.08, -1.55,  0.33,  0.54, -1.29,  0.38]
 0.68,  1.00, -1.47, -0.44, -0.66, -1.32,  0.34, -1.62]
-1.11, -0.91,  0.98,  0.78, -0.21, -1.17, -0.47,  0.14]

./matrixNorm 10 25    ->
B = [ 1.85,  0.97, -1.09,  1.66, -0.82,  1.26,  0.99,  1.12, -1.15,  0.24]
 0.96, -0.47,  1.30,  0.90,  1.02,  1.10,  0.97, -1.58,  0.99, -0.21]
 1.12, -0.33,  1.32, -0.95,  0.65,  0.98, -0.02,  0.93,  0.81, -1.41]
-0.59,  0.23, -1.10, -0.53,  0.53, -0.63, -0.98,  0.38, -0.81, -0.52]
-0.87,  1.49,  1.14, -0.94,  0.68,  0.29, -1.49,  0.47,  0.84,  1.82]
-1.12, -0.67, -0.06, -1.14,  0.67, -1.41, -1.79, -0.69,  1.40,  1.05]
 0.40, -2.04, -0.18,  0.35, -0.01, -1.62,  0.87,  1.06,  0.65, -1.73]
-0.72,  1.31, -1.00, -1.02, -1.91,  0.51,  0.57, -1.01, -0.21,  0.20]
-1.15, -0.31, -1.10,  0.35,  0.75,  0.39,  0.12,  0.70, -1.43,  0.31]
 0.12, -0.18,  0.77,  1.33, -1.57, -0.86,  0.77, -1.38, -1.10,  0.25]

./matrixNormCuda 10 25  ->
B = [ 1.85,  0.97, -1.09,  1.66, -0.82,  1.26,  0.99,  1.12, -1.15,  0.24]
 0.96, -0.47,  1.30,  0.90,  1.02,  1.10,  0.97, -1.58,  0.99, -0.21]
 1.12, -0.33,  1.32, -0.95,  0.65,  0.98, -0.02,  0.93,  0.81, -1.41]
-0.59,  0.23, -1.10, -0.53,  0.53, -0.63, -0.98,  0.38, -0.81, -0.52]
-0.87,  1.49,  1.14, -0.94,  0.68,  0.29, -1.49,  0.47,  0.84,  1.82]
-1.12, -0.67, -0.06, -1.14,  0.67, -1.41, -1.79, -0.69,  1.40,  1.05]
 0.40, -2.04, -0.18,  0.35, -0.01, -1.62,  0.87,  1.06,  0.65, -1.73]
-0.72,  1.31, -1.00, -1.02, -1.91,  0.51,  0.57, -1.01, -0.21,  0.20]
-1.15, -0.31, -1.10,  0.35,  0.75,  0.39,  0.12,  0.70, -1.43,  0.31]
 0.12, -0.18,  0.77,  1.33, -1.57, -0.86,  0.77, -1.38, -1.10,  0.25]