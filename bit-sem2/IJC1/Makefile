# Soubor:      Makefile
# Projekt:     IJC-DU1, priklad a), b)
# Autor:       Vladimir Dusek, xdusek27
# Skola:       VUT FIT 1BIT
# Datum:       25/3/2017
# Prekladac:   gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.4) 
# Popis:       $make ... sestavi vsechny varianty programu - primes, primes-i, steg-decode + *.o soubory
#              $make run ... spusti primes a primes-i s time
#              $make clean-o ... smaze vsechny *.o soubory
#              $make clean-all ... smaze vse co make vytvori
#              $make pack ... zabali vsechny soubory k odevzdani do "xdusek27.zip"


# prekladac C - gcc
CC=gcc  	


# povinne flagy + -m32 pro preklad na 32 bit os							
CFLAGS=-std=c99 -O2 -g -pedantic -Wall -Wextra


# $make
all: primes primes-i steg-decode


# *.o soubory
eratosthenes.o: eratosthenes.c eratosthenes.h error.h
	$(CC) $(CFLAGS) -c eratosthenes.c -o eratosthenes.o

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c -o error.o

ppm.o: ppm.c ppm.h error.h
	$(CC) $(CFLAGS) -c ppm.c -o ppm.o

primes.o: primes.c eratosthenes.h error.h
	$(CC) $(CFLAGS) -c primes.c -o primes.o

primes-i.o: primes.c eratosthenes.h error.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c primes.c -o primes-i.o

steg-decode.o: steg-decode.c ppm.h error.h eratosthenes.h
	$(CC) $(CFLAGS) -c steg-decode.c -o steg-decode.o


# binarky
primes: primes.o eratosthenes.o error.o
	$(CC) $(CFLAGS) primes.o eratosthenes.o error.o -lm -o primes

primes-i: primes-i.o eratosthenes.o error.o
	$(CC) $(CFLAGS) -DUSE_INLINE primes-i.o eratosthenes.o error.o -lm -o primes-i

steg-decode: steg-decode.o ppm.o eratosthenes.o error.o
	$(CC) $(CFLAGS) steg-decode.o ppm.o eratosthenes.o error.o -lm -o steg-decode


# $make run
run:
	 time ./primes 
	 time ./primes-i


# $make pack
pack:
	zip xdusek27.zip primes.c steg-decode.c eratosthenes.c error.c ppm.c bit_array.h eratosthenes.h error.h ppm.h Makefile


# $make clean-o
clean-o:
	rm -f *.o


# $make clean-all
clean-all:
	rm -f primes
	rm -f primes-i
	rm -f steg-decode
	rm -f *.o
	rm -f xdusek27.zip