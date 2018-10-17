# Filename: Makefile
# Description: Makefile for compiling CT Analysis scripts.
# Author: Latif Kabir < latif@jlab.org >
# Created: Wed Nov 15 01:00:17 2017 (-0500)
# URL: jlab.org/~latif

ROOTCINT  = rootcint
ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --glibs)
CXXFLAGS  += -fPIC -std=c++11

all: libCT

clean: libCT_clean
clear: libCT_clear


libCT: lib/libCT.so
libCT_base= $(wildcard src/*.cc)  
libCT_inc	= $(patsubst src/%.cc, ./%.h,$(libCT_base)) 
libCT_obj	= $(patsubst src/%.cc, src/%.o,$(libCT_base))

lib/libCT.so: $(libCT_obj)  src/libCT_Dict.o
	$(CXX) -o $@ $(CXXFLAGS) -shared -Wl,-soname,libCT.so $^
	cp -u src/*.pcm lib/
	@echo "                                                                        "
	@echo "-----------------------------------------------------------------------"
	@echo "Finished the compilation successfully!!!"
	@echo "Before you try it you MUST do the following:"
	@echo "1. Put the following command into your ~/.bashrc file:"
	@echo "if [ -f /path/to/libCT/bin/thislibCT.sh ]; then" 
	@echo ". /path/to/libCT/bin/thislibCT.sh"
	@echo "fi"
	@echo "2. Modify .rootrc file to include the directory that has rootlogon.C file."  
	@echo "-----------------------------------------------------------------------"
src/%.o:src/%.cc
	$(CXX) -c -o $@ $(CPPFLAGS) $(CXXFLAGS) -I$(LIB_INCLUDE) $(ROOTFLAGS) $<

src/libCT_Dict.cc:
	cd src/; $(ROOTCINT) -f libCT_Dict.cc -c $(libCT_inc) LinkDef.h

libCT_clean:
	rm -f $(libCT_obj)
	rm -f  src/*_Dict.*
	rm -f lib/libCT.so

libCT_clear:
	rm -f $(libCT_obj)
	rm -f  src/*_Dict.*


