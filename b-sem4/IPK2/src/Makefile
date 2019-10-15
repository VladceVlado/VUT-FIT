# VUT FIT
# IPK 2017/2018
# Project 2
# Variant 3: DNS Lookup tool
# Author: Vladimir Dusek, xdusek27 (2BIT)
# Date: 9/4/2018
# File: Makefile

# Usage:
#   $ make
#   $ make clean
#   $ make clean-all
#   $ make pack

# Variables:
# 	Add "-D DEBUG" to CPPFLAGS for debug mode
# 	Name of the new files add to SRC and H variables
CPPC = g++
CPPFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -funsigned-char
DEPS = dep.list
SRC = main.cpp config.cpp error.cpp lookup.cpp
H = config.h error.h lookup.h
OBJ = $(SRC:.cpp=.o)
EXEC = ipk-lookup
ARCHIVE = xdusek27.zip

.PHONY: all clean clean-all pack

all: $(DEPS) $(EXEC)

%.o : %.cpp
	$(CPPC) $(CPPFLAGS) -c $<

$(DEPS): $(SRC)
	$(CPPC) -MM $(SRC) > $(DEPS)

-include $(DEPS)

$(EXEC): $(OBJ)
	$(CPPC) $(CPPFLAGS) $^ -o $@

clean:
	rm -f $(OBJ)

clean-all:
	rm -f $(OBJ) $(EXEC) $(ARCHIVE)

pack:
	zip $(ARCHIVE) $(SRC) $(H) Makefile
