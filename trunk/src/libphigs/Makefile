CC=gcc
AR=ar
RANLIB=ranlib
RM=rm
DFLAGS=
CFLAGS= -Wall
INCFLAGS=-I../../include -I/usr/X11R6/include
OUTPATH=../../lib

.SUFFIXES: .o .c

.c.o:
	$(CC) $(DFLAGS) $(CFLAGS) $(INCFLAGS) -c $*.c

OBJS=mat_utils.o misc_utils.o css_ini.o css_stb.o css_set.o css_str.o css_el.o \
     css_inq.o css_pr.o css_ws.o phg.o phg_str.o phg_el.o phg_attr.o phg_ws.o  \
     phg_hdl.o phg_psl.o phg_err.o phg_errnum.o phg_views.o phg_col.o phg_inq.o\
     wsb.o wsgl.o wsx_gl.o

all: libphigs

libphigs: $(OBJS)
	$(AR) rv $(OUTPATH)/libphigs.a $(OBJS)
	$(RANLIB) $(OUTPATH)/libphigs.a

cleanup:
	$(RM) -rf *.o

clean:
	$(RM) -rf *.o
	$(RM) -rf $(OUTPATH)/libphigs.a

