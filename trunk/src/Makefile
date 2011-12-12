CC=gcc
RM=rm
DFLAGS=
CFLAGS= -Wall
INCFLAGS=-I../include -I/usr/X11R6/include
LDFLAGS=-L./ -L/usr/X11R6/lib
LIBS= -lGL -lGLU -lX11 -lXmu -lm

.SUFFIXES: .o .c

.c.o:
	$(CC) $(DFLAGS) $(CFLAGS) $(INCFLAGS) -c $*.c

OBJS=mat_utils.o misc_utils.o css_ini.o css_stb.o css_set.o css_str.o css_el.o \
     css_inq.o css_pr.o css_ws.o phg.o phg_str.o phg_el.o phg_attr.o phg_ws.o \
     phg_views.o phg_col.o phg_inq.o wsb.o wsgl.o

all: example1 example2

example1: $(OBJS) example1.o
	$(CC) $(LDFLAGS) -o ../bin/example1 $(OBJS) example1.o $(LIBS)

example2: $(OBJS) example2.o
	$(CC) $(LDFLAGS) -o ../bin/example2 $(OBJS) example2.o $(LIBS)

cleanup:
	$(RM) -rf *.o

clean:
	$(RM) -rf *.o
	$(RM) -rf ../bin/example1
	$(RM) -rf ../bin/example2

