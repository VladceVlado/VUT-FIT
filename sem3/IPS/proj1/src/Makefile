# Author:      Vladimir Dusek, xdusek27
# Project:     IPS - proj1, parallel grep
# University:  VUT FIT 2BIT
# Date:        13/10/2017
# Description: $ make       --- build the program
#              $ make clean --- delete binary file

NAME = pgrep
CPPC = g++
CPPFLAGS = -std=c++11 -Wall -Wextra -pedantic

all: 
	$(CPPC) $(CPPFLAGS) $(NAME).cpp -o $(NAME) -lpthread

clean:
	rm -f $(NAME)