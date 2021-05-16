/* Gaussian elimination without pivoting.
 * Compile with "mpicc gauss_mpi.c -o gauss_mpi"
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>
#include <time.h>
#include <mpi.h>

/* Program Parameters */
#define MAXN 3000  /* Max value of N changed from 2000 */
int N;  /* Matrix size */
//int numThreads; /* Number of threads */
int myid, nprocs;

/* Matrices and vectors */
float A[MAXN][MAXN], B[MAXN], X[MAXN];//changed volatile float to just float
/* A * X = B, solve for X */

/* junk */
// #define randm() 4|2[uid]&3

/* Timing variables */
  struct timeval etstart, etstop;  /* Elapsed times using gettimeofday() */
  struct timezone tzdummy;
  // clock_t etstart2, etstop2;  /* Elapsed times using times() */
  unsigned long long usecstart, usecstop;
  struct tms cputstart, cputstop;  /* CPU times for my processes */
  double MPI_start,MPI_end=0;

/* Prototype */
void gauss();  /* The function you will provide.
		* It is this routine that is timed.
		* It is called only on the parent.
		*/

/* returns a seed for srand based on the time */
unsigned int time_seed() {
  struct timeval t;
  struct timezone tzdummy;

  gettimeofday(&t, &tzdummy);
  return (unsigned int)(t.tv_usec);
}

/* Set the program parameters from the command-line arguments */
void parameters(int argc, char **argv) {
	int seed = 0;  /* Random seed */
	// char uid[32]; /*User name */

	/* Read command-line arguments */
	srand(time_seed());  /* Randomize */

	if (argc == 3) {
		seed = atoi(argv[2]);
		srand(seed);
		//numThreads=atoi(argv[3]);
    if (myid == 0){//only show output for 0 process
		  printf("Random seed = %i\n", seed);
    }
	} 
	if (argc >= 2) {
		N = atoi(argv[1]);
		if (N < 1 || N > MAXN) {
      if (myid == 0){
			  printf("N = %i is out of range.\n", N);
      }
			exit(0);
		}
	}
	else {
    if (myid ==0){
		  printf("Usage: %s <matrix_dimension> [random seed]\n",
				argv[0]);
    }
		exit(0);
	}

	/* Print parameters */
  if (myid == 0){
	  printf("\nMatrix dimension N = %i.\n", N);
  }
}

/* Initialize A and B (and X to 0.0s) */
void initialize_inputs() {
  int row, col;

  printf("\nInitializing...\n");
  for (col = 0; col < N; col++) {
    for (row = 0; row < N; row++) {
      A[row][col] = (float)rand() / 32768.0;
    }
    B[col] = (float)rand() / 32768.0;
    X[col] = 0.0;
  }

}

/* Print input matrices */
void print_inputs() {
  int row, col;

  if (N < 10) {
    printf("\nA =\n\t");
    for (row = 0; row < N; row++) {
      for (col = 0; col < N; col++) {
	printf("%5.2f%s", A[row][col], (col < N-1) ? ", " : ";\n\t");
      }
    }
    printf("\nB = [");
    for (col = 0; col < N; col++) {
      printf("%5.2f%s", B[col], (col < N-1) ? "; " : "]\n");
    }
  }
}

void print_X() {
  int row;

  if (N < 100) {
    printf("\nX = [");
    for (row = 0; row < N; row++) {
      printf("%5.2f%s", X[row], (row < N-1) ? "; " : "]\n");
    }
  }
}

int main(int argc, char **argv) {
  /*MPI initialization */
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);
  
  /* Process program parameters */
  parameters(argc, argv);
 
  /* Gaussian Elimination */
  gauss();

  if(myid==0){
    /* Stop Clock */
    gettimeofday(&etstop, &tzdummy);
    times(&cputstop);
    printf("Stopped clock.\n");
    usecstart = (unsigned long long)etstart.tv_sec * 1000000 + etstart.tv_usec;
    usecstop = (unsigned long long)etstop.tv_sec * 1000000 + etstop.tv_usec;

    /* Display output */
    print_X();

    /* Display timing results */
    printf("\nElapsed time = %g ms.\n",
    (float)(usecstop - usecstart)/(float)1000);

    printf("Elapsed MPI-Timer time = %f seconds\n", MPI_end-MPI_start);//Not sure if this timer is needed since the included code has a similar timer

//Old CPU time Outputs
  /* 
    printf("(CPU times are accurate to the nearest %g ms)\n",
    1.0/(float)CLOCKS_PER_SEC * 1000.0);
    printf("My total CPU time for parent = %g ms.\n",
    (float)( (cputstop.tms_utime + cputstop.tms_stime) -
        (cputstart.tms_utime + cputstart.tms_stime) ) /
    (float)CLOCKS_PER_SEC * 1000);
    printf("My system CPU time for parent = %g ms.\n",
    (float)(cputstop.tms_stime - cputstart.tms_stime) /
    (float)CLOCKS_PER_SEC * 1000);
    printf("My total CPU time for child processes = %g ms.\n",
    (float)( (cputstop.tms_cutime + cputstop.tms_cstime) -
        (cputstart.tms_cutime + cputstart.tms_cstime) ) /
    (float)CLOCKS_PER_SEC * 1000);
        /* Contrary to the man pages, this appears not to include the parent */
    printf("--------------------------------------------\n");
    exit(0);
  }
  MPI_Finalize();
}

/* ------------------ Above Was Provided --------------------- */


/****** You will replace this routine with your own parallel version *******/
/* Provided global variables are MAXN, N, A[][], B[], and X[],
 * defined in the beginning of this code.  X[] is initialized to zeros.
 */
void gauss() {
  int norm, row, col;  /* Normalization row, and zeroing element row and col */
  float multiplier;
  int i, p;

  MPI_Request request;
	MPI_Status status;
  
  if(myid == 0){
    printf("Number of Processes: %d\n", nprocs);

    /* Initialize A and B */
    initialize_inputs();

    /* Print input matrices */
    print_inputs();

    /* Start Clock */
    printf("\nStarting clock.\n");
    gettimeofday(&etstart, &tzdummy);
    times(&cputstart);
    MPI_start = MPI_Wtime();
    printf("Computing With MPI.\n");
  }

  /*Work split and send section */
  if(myid == 0){//processor 0 will send row data to other processes
    for(i = 0; i < N; i++){
      p = i % nprocs;//every processor will get alternating rows of the matrix to work on. 
      if (p !=0){
        MPI_Send(A[i], N, MPI_FLOAT, p, i, MPI_COMM_WORLD);
			  MPI_Send(&B[i], 1, MPI_FLOAT, p, i+N, MPI_COMM_WORLD);
      }
    }
  }
  else{//receive row data sent by processor 0
    for(i = 0; i< N; i++){
      p = i % nprocs;
      if(p == myid){
        MPI_Recv(A[i], N, MPI_FLOAT, 0, i, MPI_COMM_WORLD, &status);		
				MPI_Recv(&B[i], 1, MPI_FLOAT, 0, i+N, MPI_COMM_WORLD, &status);
      }
    }
  }
  
  /* Gaussian elimination */
  for (norm = 0; norm < N - 1; norm++) {
      //every process will get the norm row from the 0 processor at the start
      MPI_Bcast(A[norm], N, MPI_FLOAT, 0, MPI_COMM_WORLD);
      MPI_Bcast(&B[norm], 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    /*Gaussian reduction section */
      for (row = norm + 1; row < N; row++) {//work will essentially only be done on the first row = norm + 1. Every other row will be either empty or in another processor's matrix
        if(row % nprocs == myid){//Processors will only do rows that correspond to their id
          multiplier = A[row][norm] / A[norm][norm];
          for (col = norm; col < N; col++) {
            A[row][col] -= A[norm][col] * multiplier;
          }
          B[row] -= B[norm] * multiplier;
          if(row == norm + 1 && myid !=0){//only send back row data when it is right before the norm line to processor 0 to do back substitution
            MPI_Send(A[row], N, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);						    		
				    MPI_Send(&B[row], 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
          }
        }

        /*Receive section after the reduction calculations */
        if(myid ==0 && row ==norm + 1 && row % nprocs !=0){
          MPI_Recv(A[row], N, MPI_FLOAT, (norm + 1) % nprocs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);// (norm + 1)% nprocs eliminates the need to use a "for loop" for #p processes
          MPI_Recv(&B[row], 1, MPI_FLOAT, (norm + 1) % nprocs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
  }// end of norm loop
  MPI_Barrier(MPI_COMM_WORLD);//wait for all processors to finish before going to back substitution

  /* Back substitution */
  if (myid == 0){ //At this point processsor 0 will have gotten all the data back from the other processors and will do back substitution alone
    for (row = N - 1; row >= 0; row--) {
        X[row] = B[row];
        for (col = N-1; col > row; col--) {
        X[row] -= A[row][col] * X[col];
        }
        X[row] /= A[row][row];
    }
  }
  MPI_end= MPI_Wtime();//This is the end of the MPI algorithm version of gauss elimination
}