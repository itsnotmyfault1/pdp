#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include "mpi.h"
#include <omp.h>

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

void solve(double **A, int n)
{
   int convergence=FALSE;
   double diff,tdiff, tmp;
   int i,j, iters=0;
   int for_iters, chunk_size, biggers,mywork,mystart;
//start doing mpi things here
int MyRank, world_size;
MPI_Status status;
MPI_Init(0,0);
MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
MPI_Comm_size(MPI_COMM_WORLD, &world_size);

//for static allocation
chunk_size=floor(n/world_size); //the base chunk size
biggers=n-chunk_size*world_size; //if your rank is < biggers, you work on chunk_size+1 rows


   for (for_iters=1;for_iters<21;for_iters++) 
   { 
     diff = 0.0;
     tdiff = 0.0;
//master
if(MyRank==0){
  if(MyRank<biggers){int mywork=chunk_size+1;} else{int mywork=chunk_size;}
  //MPI_Send(*message, size, type, dest, tag, comm)
  MPI_Send(&A[mywork][0],(N+2)*sizeof(double *), MPI_DOUBLE, 1, 1, MPI_COMM_WORLD);
  MPI_Recv(&A[mywork+1][0],(N+2)*sizeof(double *), MPI_DOUBLE, 2, 1, MPI_COMM_WORLD, &status);
     for (i=1;i<=mywork;i++)
     {
       for (j=1;j<n;j++)
       {
         tmp = A[i][j];
         A[i][j] = 0.2*(A[i][j] + A[i][j-1] + A[i-1][j] + A[i][j+1] + A[i+1][j]);
         diff += fabs(A[i][j] - tmp);
       }
     }
} else{
//slaves
//if rank<biggers, work on chunksize+1
if(MyRank<biggers){int mywork=chunk_size+1; int mystart=(MyRank+1)*(mywork);} 
else{int mywork=chunk_size; int mystart=(MyRank+1)*(mywork)+biggers;}
  //send your beginning stuff to your upstairs neighbor, grab his shit too
  MPI_Send(&A[mystart][0],(N+2)*sizeof(double *), MPI_DOUBLE, MyRank-1, 1, MPI_COMM_WORLD);
  MPI_Recv(&A[mystart-1][0],(N+2)*sizeof(double *), MPI_DOUBLE, MyRank-1, 1, MPI_COMM_WORLD, &status);
  //send your ending stuff to your downstairs neighbor, grab his too, unless you're the last guy
  if(MyRank!=world_size-1){
  MPI_Send(&A[mystart+mywork][0],(N+2)*sizeof(double *), MPI_DOUBLE, MyRank+1, 1, MPI_COMM_WORLD);
  MPI_Recv(&A[mystart+mywork+1][0],(N+2)*sizeof(double *), MPI_DOUBLE, MyRank+1, 1, MPI_COMM_WORLD, &status);}
     for (i=mystart;i<=mystart+mywork;i++)
     {
       for (j=1;j<n;j++)
       {
         tmp = A[i][j];
         A[i][j] = 0.2*(A[i][j] + A[i][j-1] + A[i-1][j] + A[i][j+1] + A[i+1][j]);
         diff += fabs(A[i][j] - tmp);
       }
     }
}
     iters++;

     MPI_Reduce(&diff, &tdiff, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD);
     if (tdiff/((double)N*(double)N) < Tolerance)
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

   solve(A, N);

   t_end=usecs();

   time = ((double)(t_end-t_start))/1000000;
   printf("Computation time = %f\n", time);

}
