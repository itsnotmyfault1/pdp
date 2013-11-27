/* mandelbrotdynamic.c */

#include <mpi.h> /*header for mpi */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define		X_RESN	800       /* x resolution */
#define		Y_RESN	800       /* y resolution */

/*COnstants for the message passing */
#define		WORK_TAG 1	/* "work" (master to slave) */
#define		DATA_TAG 2	/* "data" (slave to master) */
#define		STOP_TAG 3 	/* "stop" (master to slave) */

typedef struct complextype
	{
        float real, imag;
	} Compl;


int main (int argc, char *argv[])
{
	Window		win;                            /* initialization for a window */
	unsigned
	int             width, height,                  /* window size */
                        x, y,                           /* window position */
                        border_width,                   /*border width in pixels */
                        display_width, display_height,  /* size of screen */
                        screen;                         /* which screen */

	char            *window_name = "Mandelbrot Set", *display_name = NULL;
	GC              gc;
	unsigned
	long		valuemask = 0;
	XGCValues	values;
	Display		*display;
	XSizeHints	size_hints;
	Pixmap		bitmap;
	XPoint		points[800];
	FILE		*fp, *fopen ();
	char		str[100];
	
	XSetWindowAttributes attr[1];

       
       
	/* connect to Xserver */

	if (  (display = XOpenDisplay (display_name)) == NULL ) {
	   fprintf (stderr, "drawon: cannot connect to X server %s\n",
				XDisplayName (display_name) );
	exit (-1);
	}
	
	/* get screen size */

	screen = DefaultScreen (display);
	display_width = DisplayWidth (display, screen);
	display_height = DisplayHeight (display, screen);

	/* set window size */

	width = X_RESN;
	height = Y_RESN;

	/* set window position */

	x = 0;
	y = 0;

        /* create opaque window */

	border_width = 4;
	win = XCreateSimpleWindow (display, RootWindow (display, screen),
				x, y, width, height, border_width, 
				BlackPixel (display, screen), WhitePixel (display, screen));

	size_hints.flags = USPosition|USSize;
	size_hints.x = x;
	size_hints.y = y;
	size_hints.width = width;
	size_hints.height = height;
	size_hints.min_width = 300;
	size_hints.min_height = 300;
	
	XSetNormalHints (display, win, &size_hints);
	XStoreName(display, win, window_name);

        /* create graphics context */

	gc = XCreateGC (display, win, valuemask, &values);

	XSetBackground (display, gc, WhitePixel (display, screen));
	XSetForeground (display, gc, BlackPixel (display, screen));
	XSetLineAttributes (display, gc, 1, LineSolid, CapRound, JoinRound);

	attr[0].backing_store = Always;
	attr[0].backing_planes = 1;
	attr[0].backing_pixel = BlackPixel(display, screen);

	XChangeWindowAttributes(display, win, CWBackingStore | CWBackingPlanes | CWBackingPixel, attr);

	XMapWindow (display, win);
	XSync(display, 0);

int comm_sz;  
int my_rank;  
MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
MPI_Status status;
long *message = malloc((width+1) * sizeof(*message));

/* Mandlebrot variables */
        int i, j, k;
        Compl	z, c;
        float	lengthsq, temp;

int nextrow = 0;	/* Next row to give to slave */
int working = 0;	/* Count of slaves still working */

if (my_rank == 0) {
/* Assigns each slave a row to work on */
for(i = 0; i < comm_sz; i++) {
	MPI_Send(&nextrow, 1, MPI_INT, i+1, WORK_TAG, MPI_COMM_WORLD);
	nextrow++;
	working++;
}

/* REceive data from slave */
while (working > 0) {
	MPI_Recv(message, Y_RESN+1, MPI_LONG, MPI_ANY_SOURCE, DATA_TAG, MPI_COMM_WORLD, &status); /*slave delivers the message */
	working--;  /*Slave finished work */

  if (nextrow < X_RESN){
	/*determines if more work needs to be done then  assigns more work*/
	MPI_Send(&nextrow,1,MPI_INT,i+1, WORK_TAG, MPI_COMM_WORLD);
	nextrow++;  	 
	working++;/*slave recieves work*/
	}
  else MPI_Send(&nextrow, 0, MPI_INT, i+1, STOP_TAG, MPI_COMM_WORLD);

int thisrow = message[0];

for(j = 0; j < Y_RESN; j++) 
  if(message[j+1] == 100)
	XDrawPoint(display, win, gc, j, thisrow);
}
}

else {
int row;
while((MPI_Recv(row, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status) == MPI_SUCCESS) && (status.MPI_TAG == WORK_TAG)) {
	message[0] = row;
        for(j=0; j < Y_RESN; j++) {

          z.real = z.imag = 0.0;
          c.real = ((float) j - 400.0)/200.0;               /* scale factors for 800 x 800 window */
	  c.imag = ((float) i - 400.0)/200.0;
          k = 0;

          do  {                                             /* iterate for pixel color */

            temp = z.real*z.real - z.imag*z.imag + c.real;
            z.imag = 2.0*z.real*z.imag + c.imag;
            z.real = temp;
            lengthsq = z.real*z.real+z.imag*z.imag;
            k++;

          } while (lengthsq < 4.0 && k < 100);

        message[j+1] = k;

        }
/* Send data from this row */
MPI_Send(message, Y_RESN, MPI_LONG, 0, DATA_TAG, MPI_COMM_WORLD);
}
}
MPI_Finalize();	  /*ends mpi */
	XFlush (display);
	sleep (30);

	/* Program Finished */

}


