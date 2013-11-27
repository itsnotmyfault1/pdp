#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main(int argc,char *argv[])
{

  int rank, i, tag=1;
  char message[1048576];
  MPI_Status stat;
  double tot, t1, t2;
  int count;
   
    MPI_Init(&argc, &argv);
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Barrier(MPI_COMM_WORLD);
         
          if (rank == 0)
          {
                count = 0;
                    int c = 0;
                        while(count < 1048577)
                        {
                              t1 = MPI_Wtime();
                                  for(i = 1; i <= 100; i++)
                                  {
                                        /* send to process 1 */
                                       MPI_Send(message, count, MPI_CHAR, 1, tag, MPI_COMM_WORLD);
                                          MPI_Recv(message, count, MPI_CHAR, 1, tag, MPI_COMM_WORLD, &stat);
                                  }

                                      t2 = MPI_Wtime();
                                          tot = t2 - t1;
                                          printf("%d  %f  %f\n", count, tot/200.,(float)(2 * 100 * count)/tot);
                                             
                                              count = (int) pow(2,(double)c);
                                                  c++;
                        }
          }
          else /* myRank == 1 */
          {

                 count = 0;
                     double c = 0;
                         while(count < 1048577)
                         {
                               t1 = MPI_Wtime();
                                   for(i = 1; i <= 100; i++)
                                   {

                                         /* receive from process 0 */
                                         MPI_Recv(message, count, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &stat);
                                             MPI_Send(message, count, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
                                   }
                                   t2 = MPI_Wtime();
                                       tot = t2 - t1;
                                           count = (int) pow(2,(double)c);
                                               c++;
                         }
          }
           

            MPI_Barrier(MPI_COMM_WORLD);
              MPI_Finalize();
}

