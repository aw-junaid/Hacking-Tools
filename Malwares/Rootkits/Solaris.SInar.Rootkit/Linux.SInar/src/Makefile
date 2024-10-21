#
# SInAR Makefile - Copyright as per normal in LICENSE
# WOO HOO!!! SUN finally give us a FREE compiler by default!
CC=gcc
#CFLAGS= -Wall -m64 -D_KERNEL -DSVR4 -DSOL2 -c 
CFLAGS= -Wall -D_KERNEL -DSVR4 -DSOL2 -D__i386 -c
#LFLAGS= -64 -r
LFLAGS= -r
all: sinar
	 
clean:
	rm -f *.o sinar *.*~
	
sinar:
	$(CC) $(CFLAGS) sinar.c -o sinar.o
	ld $(LFLAGS) sinar.o -o sinar
