CC=gcc
RM=rm
DFLAGS=
CFLAGS= -Wall
INCFLAGS=-I../../include -I/usr/X11R6/include
LDFLAGS=-L../../lib -L/usr/X11R6/lib
LIBS= -lGL -lGLU -lXaw -lXt -lX11 -lXmu -lm -lphigs
OUTPATH=../../bin

.SUFFIXES: .o .c

.c.o:
	$(CC) $(DFLAGS) $(CFLAGS) $(INCFLAGS) -c $*.c

all: example1 example2 example3 example4

example1: example1.o
	$(CC) $(LDFLAGS) -o $(OUTPATH)/example1 $(OBJS) example1.o $(LIBS)

example2: $(OBJS) example2.o
	$(CC) $(LDFLAGS) -o $(OUTPATH)/example2 $(OBJS) example2.o $(LIBS)

example3: $(OBJS) example3.o
	$(CC) $(LDFLAGS) -o $(OUTPATH)/example3 $(OBJS) example3.o $(LIBS)

example4: $(OBJS) example4.o
	$(CC) $(LDFLAGS) -o $(OUTPATH)/example4 $(OBJS) example4.o $(LIBS)

cleanup:
	$(RM) -rf *.o

clean:
	$(RM) -rf *.o
	$(RM) -rf $(OUTPATH)/example1
	$(RM) -rf $(OUTPATH)/example2
	$(RM) -rf $(OUTPATH)/example3
	$(RM) -rf $(OUTPATH)/example4

