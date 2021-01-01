# Title:        Graph Parralel Radio Coloring
# Author:       Vladimir Dusek     <xdusek27 AT stud.fit.vutbr.cz>
# Author:       Patrik Goldschmidt <xgolds00 AT stud.fit.vutbr.cz>
# Subject:      GAL
# Organization: Brno University of Technology, Czech Republic
# Date:         12-07-2020, last rev. 12-15-2020

##################################  USAGE:  #################################
# make        Compiles project's binary files.
# make debug  Compiles project's debug version.
# make devel  Compiles project's development version.
# make run    Compiles and runs the project on the example problem.
# make pack   Packs source files and headers into the .tar.gz archive.
# make zip    Packs source files and headers into the .zip archive.
# make clean  Cleans object files and binaries.

NAME=gal
AUTHOR=xgolds00
SRCDIR=.

MODULES=main argparse colorParallel colorSequential graphUtils io matrix \
        radiocolor
ALLFILES=$(SRCDIR)/* Makefile
OBJFILES=$(addprefix $(SRCDIR)/,$(addsuffix .o,$(MODULES)))

CC=g++
CXX=g++
CXXFLAGS=-std=c++17 -O2 -fopenmp
LIBS=
DEBUGFLAGS=-g -DDEBUG
DEVELFLAGS=-D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS -pedantic \
           -Wall -Wextra -Wconversion -Wcast-qual -Wcast-align -Wshadow \
           -Wstrict-aliasing=1 -Wswitch-enum -Wundef -Wpointer-arith \
           -Wformat-security -Wvla -Wformat=2 -Winit-self -Wfloat-equal \
           -Wwrite-strings -Wredundant-decls -Wstrict-overflow=2 \
			  -Wimplicit-fallthrough

$(NAME): $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(NAME)

devel: CXXFLAGS += $(DEVELFLAGS)
devel: $(NAME)

paropt: CXXFLAGS += -DPARALLEL_OPTIMIZED
paropt: $(NAME)

run: $(NAME)
run:
	./$(NAME)

clean:
	rm -f $(NAME) $(OBJFILES) $(AUTHOR).tar.gz $(AUTHOR).zip

pack: $(ALLFILES)
	tar -czf $(AUTHOR).tar.gz $^

zip: $(ALLFILES)
	zip $(AUTHOR).zip $^

czip: clean
czip: zip

.PHONY: debug devel run clean pack zip czip


main.o: main.cpp argparse.o colorParallel.o colorSequential.o graphUtils.o \
        matrix.o radiocolor.o

argparse.o: argparse.cpp argparse.hpp

radiocolor.o: radiocolor.cpp radiocolor.hpp matrix.o

colorSequential.o: colorSequential.cpp colorSequential.hpp radiocolor.o

colorParallel.o: colorParallel.cpp colorParallel.hpp radiocolor.o

matrix.o: matrix.cpp matrix.hpp

graphUtils.o: graphUtils.cpp graphUtils.hpp matrix.o
