/* mandelbrotstatic.c */

#include <mpi.h> /*header for mpi*/
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
                 /* Need this for the six functions */


#define		X_RESN	800       /* x resolution */
#define		Y_RESN	800       /* y resolution */

typedef struct complextype
	{
        float real, imag;
	} Compl;


int main ()
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
      	 /* declaring everything needed for passingand recieving  messages */
int comm_sz;  
int my_rank = 800;  
MPI_Init(NULL,NULL);
MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);  
MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
MPI_Status status;
long *message = malloc((width+1) * sizeof(*message));

/* Mandlebrot variables */
        int i, j, k;
        Compl	z, c;
        float	lengthsq, temp;


        /* Calculate and draw points */
if(my_rank == 0)  /*master process*/
{
	for(i=0; i < X_RESN; i++) /*loops through 800 times */
	{
		MPI_Recv(message, Y_RESN+1, MPI_LONG, i+1, 1, MPI_COMM_WORLD, &status); /*800 messages recieved from slaves */
		int thisrow = message[0];
		
		for(j=0; j < X_RESN; j++) {
			if(message[j+1] == 100)
				XDrawPoint(display, win, gc, j, thisrow);
		}
	}
}
else /*each slave is responsible for a row */
{
	for(i= (my_rank - 1); i < X_RESN; i++)
	{
		message[0] = i;
		for(j=0; j < Y_RESN; j++)
		{
			z.real = z.imag = 0.0;
          		c.real = ((float) j - 400.0)/200.0;               /* 				scale factors for 800 x 800 window */
	  		c.imag = ((float) i - 400.0)/200.0;
          		k = 0;	
		do {
			
          	  temp = z.real*z.real - z.imag*z.imag + c.real;
          	  z.imag = 2.0*z.real*z.imag + c.imag;
         	   z.real = temp;
         	   lengthsq = z.real*z.real+z.imag*z.imag;
          	  k++;

         	 } while (lengthsq < 4.0 && k < 100);
		message[j+1] = k;
		}
		MPI_Send(message, Y_RESN+1, MPI_LONG, 0, 1, MPI_COMM_WORLD);
	}
}
MPI_Finalize(); /*ends mpi */
	
	XFlush (display);
	sleep (30);

	/* Program Finished */

}

