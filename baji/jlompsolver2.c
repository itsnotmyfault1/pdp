#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>

#define Tolerance 0.00001
#define TRUE 1
#define FALSE 0

#define N 10000

double ** A;

int initialize (double **A, int n)
{
   int i,j;

   for (j=0;j<n+1;j++){
     A[0][j]=1.0;
   }
   for (i=1;i<n+1;i++){
      A[i][0]=1.0;
      for (j=1;j<n+1;j++) A[i][j]=0.0;
   }

}

void solve(double **A, int n, int tid)
{
   int convergence=FALSE;
   double diff, tmp;
   int i,j, nthreads, iters=0;
   int for_iters;

   {
#pragma omp single
     {
       nthreads = omp_get_num_threads();
       printf("Starting with %d threads\n", nthreads);
     }
     
     for (for_iters=1;for_iters<21;for_iters++) 
       {
	diff = 0.0;
 
#pragma omp for private(tmp, diff, j)  schedule(static)
	 for (i=1;i<n;i++)
	  { 
	     for (j=1;j<n;j++)
	       {
		 tmp = A[i][j];
		 A[i][j] = 0.2*(A[i][j] + A[i][j-1] + A[i-1][j] + A[i][j+1] + A[i+1][j]);
		 diff += fabs(A[i][j] - tmp);
	       }
	   }

	 iters++;
	 
	 if (diff/((double)N*(double)N) < Tolerance)
	   convergence=TRUE;
	 
       } /*for*/
   
//     printf("Thread %d completed\n", tid);

     }
}

long usecs (void)
{
  struct timeval t;

  gettimeofday(&t,NULL);
  return t.tv_sec*1000000+t.tv_usec;
}


int main(int argc, char * argv[])
{
   int i, Tid;
   long t_start,t_end;
   double time;

   A = malloc((N+2) * sizeof(double *));
   for (i=0; i<N+2; i++) {
	A[i] = malloc((N+2) * sizeof(double)); 
   }

   initialize(A, N);

   t_start=usecs();
#pragma omp parallel
   {
   Tid = omp_get_thread_num();
   solve(A, N, Tid);
   }
   t_end=usecs();

   time = ((double)(t_end-t_start))/1000000;
   printf("Computation time = %f\n", time);

}
