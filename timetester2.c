#include <mpi.h>
#include <stdio.h>
#include <math.h>


int main(int argc, char **argv)
{
int i, count, j;
int tag=1;
char message[1048577];
double start, elapsed;
float x, y, z;

//these guys are needed all the time
int MyRank, world_size;
MPI_Status status;
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
MPI_Comm_size(MPI_COMM_WORLD, &world_size);

/* Ping-Pong message */
if(MyRank == 0) {
  
  count=0;
  i=0;
  while(count <1048577){
   start=MPI_Wtime();
   for(j=1; j<=100; j++){ //send and recieve a bunch and average over all of them
     /* send to process 1 */
     MPI_Send(message, count, MPI_CHAR, 1, 0, MPI_COMM_WORLD); //pings are tagged 0, pongs are tagged 1
     /* receive from process 1 */
     MPI_Recv(message, count, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &status);
     MPI_Barrier(MPI_COMM_WORLD);
   }
   elapsed =MPI_Wtime() - start;
   printf("%d %f %f\n", count, elapsed/200.,(float)(200*count)/elapsed); //bytes sent, average time/message, average bandwidth
   count=(int) pow(2,(double)i);
   i++;
  }
}
else /* myRank == 1 */{
       /* receive from process 0 */
       //i probably should have made a bunch of this junk into a function..
 count=0;
 i=0;
 while(count<1048577){
   for(j=1; j<=100; j++){
       MPI_Recv(message, count, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
     /* send to process 0 */
       MPI_Send(message, count, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
       MPI_Barrier(MPI_COMM_WORLD);
     }
   count=(int) pow(2,(double)i);
   i++;
 }
}
MPI_Barrier(MPI_COMM_WORLD);
MPI_Finalize();
}

