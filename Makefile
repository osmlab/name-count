#------------------------------------------------------------------------------
#
#  Makefile for Osmium examples
#
#------------------------------------------------------------------------------
#
#  You can set several environment variables before running make if you don't
#  like the defaults:
#
#  CXX                - Your C++ compiler.
#  CPLUS_INCLUDE_PATH - Include file search path.
#  CXXFLAGS           - Extra compiler flags.
#  LDFLAGS            - Extra linker flags.
#
#------------------------------------------------------------------------------

CXXFLAGS += -O3
#CXXFLAGS += -g
CXXFLAGS += -std=c++11 -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
CXXFLAGS += -I../include

OS:=$(shell uname -s)
ifeq ($(OS),Darwin)
	CXXFLAGS += -stdlib=libc++
	LDFLAGS += -stdlib=libc++
endif

CXXFLAGS_GEOS     := $(shell geos-config --cflags)
CXXFLAGS_LIBXML2  := $(shell xml2-config --cflags)
CXXFLAGS_OGR      := $(shell gdal-config --cflags)
CXXFLAGS_WARNINGS := -Wall -Wextra -pedantic -Wredundant-decls -Wdisabled-optimization -Wctor-dtor-privacy -Wnon-virtual-dtor -Woverloaded-virtual -Wsign-promo -Wold-style-cast

LIB_EXPAT  := -lexpat
LIB_PBF    := -pthread -lz -lprotobuf-lite -losmpbf
LIB_GD     := -lgd -lz -lm
LIB_GEOS   := $(shell geos-config --libs)
LIB_OGR    := $(shell gdal-config --libs)
LIB_SHAPE  := -lshp $(LIB_GEOS)
LIB_XML2   := $(shell xml2-config --libs)

# this links libboost_program_options statically, because for some reason
# it always wants the exact version which might not be available on a
# different host
LIB_PRGOPT := -Wl,-Bstatic -lboost_program_options -Wl,-Bdynamic

PROGRAMS := \
    name_count \
    name_unicode_geo

.PHONY: all clean

all: $(PROGRAMS)

name_count: name_count.cpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_WARNINGS) $(CXXFLAGS_LIBXML2) -o $@ $< $(LDFLAGS) $(LIB_EXPAT) $(LIB_PBF) $(LIB_XML2)

name_unicode_geo: name_unicode_geo.cpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_WARNINGS) $(CXXFLAGS_LIBXML2) -o $@ $< $(LDFLAGS) $(LIB_EXPAT) $(LIB_PBF) $(LIB_XML2)

clean:
	rm -f *.o core $(PROGRAMS)
