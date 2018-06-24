# Name of the final executable of the test-suite
EXEC = test-suite

# Files specified in here are going to be compiled into final
# test-suite executable. Files that are going to be used to build the
# project must be specified manually.
SRC = TestSuite.cpp

# Build specifying variables
DEPS = dep.list
CC = g++
CFLAGS = -std=c++11 -pedantic -g
CPPUTEST = CppUTest

# Transforms *.c into *.o
OBJ = $(SRC:.cpp=.o)

%.o: %.cpp
	$(CC) -I $(SRC_DIR) $(CFLAGS) -c $<


all: $(DEPS) $(EXEC)

# Dependencies of every test file in project.
# File dependencies are generated whenever
# a file of the project is modified.
# Generated dependencies are inserted into Makefile.
$(DEPS): $(SRC)
	g++ -I $(SRC_DIR) -MM $(SRC) > $(DEPS)

-include $(DEPS)

$(EXEC): $(OBJ) $(SRC_OBJ)
	$(CC) -I $(SRC_DIR) $(CFLAGS) $^ -l$(CPPUTEST) -o $@
