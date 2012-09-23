CXX=g++
RM=rm
DFLAGS=
CFLAGS= -Wall
INCFLAGS=-I../../include -I/usr/X11R6/include
LDFLAGS=-L../../lib -L/usr/X11R6/lib
LIBS= -lGL -lGLU -lGLw -lXm -lXaw -lXt -lX11 -lXmu -lm -lphigs
OUTPATH=../../bin

.SUFFIXES: .o .c

.C.o:
	$(CXX) $(DFLAGS) $(CFLAGS) $(INCFLAGS) -c $*.C

OBJS = DrawingArea.o Main.o

all: Revolver

Revolver: $(OBJS)
	$(CXX) $(LDFLAGS) -o $(OUTPATH)/Revolver $(OBJS) $(LIBS)

cleanup:
	$(RM) -rf *.o

clean:
	$(RM) -rf *.o
	$(RM) -rf $(OUTPATH)/Revolver

