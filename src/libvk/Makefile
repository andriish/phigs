CXX=g++
AR=ar
RANLIB=ranlib
RM=rm
DFLAGS=
CXXFLAGS= -Wall
INCFLAGS=-I../../include -I/usr/X11R6/include
OUTPATH=../../lib

.SUFFIXES: .o .c

.C.o:
	$(CXX) $(DFLAGS) $(CXXFLAGS) $(INCFLAGS) -c $*.C

OBJS= VkComponent.o VkComponentList.o VkApp.o VkSimpleWindow.o

all: libvk

libvk: $(OBJS)
	$(AR) cr $(OUTPATH)/libvk.a $(OBJS)
	$(RANLIB) $(OUTPATH)/libvk.a

cleanup:
	$(RM) -rf *.o

clean:
	$(RM) -rf *.o
	$(RM) -rf $(OUTPATH)/libvk.a

