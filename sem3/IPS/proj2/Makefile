# Author:      Vladimir Dusek, xdusek27
# Project:     IPS - proj2, thread memory allocator
# University:  VUT FIT 2BIT
# Date:        15/11/2017
# Note:        Muze se hodit vypnuti randomizace pameti:
#              $ sudo sysctl kernel.randomize_va_space=0

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra

all:
	$(CC) $(CFLAGS) tmal.c -c
	$(CC) $(CFLAGS) test_tmal.c -c
	$(CC) -o test_tmal test_tmal.o tmal.o

clean:
	rm -f tmal.o test_tmal.o test_tmal