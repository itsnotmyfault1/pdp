#include <stdio.h>
#include <math.h>
#include <sys/time.h>

#define Tolerance 0.00001
#define TRUE 1
#define FALSE 0

#define N 500

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

void solve(double **A, int n, int MyRank, int world_size)
{
   int convergence=FALSE;
   double diff, tmp;
   int i,j, iters=0;
   int for_iters;


   for (for_iters=1;for_iters<21;for_iters++) 
   { 
     diff = 0.0;

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
}


long usecs (void)
{
  struct timeval t;

  gettimeofday(&t,NULL);
  return t.tv_sec*1000000+t.tv_usec;
}


int main(int argc, char * argv[])
{
   int i;
   long t_start,t_end;
   double time;

   A = malloc((N+2) * sizeof(double *));
   for (i=0; i<N+2; i++) {
	A[i] = malloc((N+2) * sizeof(double)); 
   }

   initialize(A, N);
   t_start=usecs();
//start doing mpi things here
int MyRank, world_size;
MPI_Status status;
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
MPI_Comm_size(MPI_COMM_WORLD, &world_size);
   solve(A, N, MyRank, world_size);



   t_end=usecs();

   time = ((double)(t_end-t_start))/1000000;
   printf("Computation time = %f\n", time);

}
