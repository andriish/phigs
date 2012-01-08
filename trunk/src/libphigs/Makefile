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

OBJS=ut.o ut_mat.o ut_list.o ut_htab.o css_ini.o css_stb.o css_set.o css_str.o \
     css_el.o css_inq.o css_pr.o css_ws.o phg.o phg_str.o phg_el.o phg_attr.o  \
     phg_ws.o phg_hdl.o phg_psl.o phg_err.o phg_errnum.o phg_col.o phg_inq.o   \
     phg_tr.o phg_wst.o wsb.o wsb_lut.o wsgl.o wsgl_hatch.o wsx_gl.o sin.o     \
     sinq.o sin_ws.o sin_cvs.o sin_val.o sin_cho.o sin_str.o sin_dev.o         \
     sin_evt.o ws_inp.o phg_inp.o

all: libphigs

libphigs: $(OBJS)
	$(AR) rv $(OUTPATH)/libphigs.a $(OBJS)
	$(RANLIB) $(OUTPATH)/libphigs.a

cleanup:
	$(RM) -rf *.o

clean:
	$(RM) -rf *.o
	$(RM) -rf $(OUTPATH)/libphigs.a

