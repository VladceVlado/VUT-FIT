# Soubor:      Makefile
# Projekt:     Projekt 2 - Sprava procesu
# Predmet:     Operacni systemy (IOS)
# Autor:       Vladimir Dusek, xdusek27
# Skola:       VUT FIT 1BIT
# Datum:       18/4/2017
# Prekladac:   gcc 5.4.0
# Popis:       $make ... sestavi program
#              $make clean ... smaze *.o soubory
#              $make clean-all ... smaze vse co make vytvori
#              $make pack ... zabali vsechny soubory k odevzdani do "proj2.zip"


# prekladac
CC = gcc

# povinne flagy								
CFLAGS = -std=gnu99 -Wall -Wextra -pedantic -Werror

# $make
all: proj2

# *.o soubory
proj2.o: proj2.c
	$(CC) $(CFLAGS) -c proj2.c -o proj2.o

# binarka
proj2: proj2.o
	$(CC) $(CFLAGS) proj2.o -o proj2 -lpthread

# $make pack
pack:
	zip proj2.zip proj2.c proj2.h Makefile

# $make clean-o
clean:
	rm -f *.o

# $make clean-all
clean-all:
	rm -f proj2
	rm -f *.o
	rm -f proj2.zip
