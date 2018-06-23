# Soubor:      Makefile
# Projekt:     IJC-DU2, priklad 1), 2)
# Autor:       Vladimir Dusek, xdusek27
# Skola:       VUT FIT 1BIT
# Datum:       3/4/2017
# Prekladac:   gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.4)
# Popis:       $make ... sestavi vsechny varianty programu - tail, tail2, wordcount, 
#                        wordcount-dynamic, libhtab.a, libhtab.so + *.o soubory
#              $make clean-o ... smaze vsechny *.o soubory
#              $make clean-all ... smaze vse co make vytvori
#              $make pack ... zabali vsechny soubory k odevzdani do "xdusek27.zip"
# Upozorneni:  Pri spusteni wordcount-dynamic je potreba nastavit LD_LIBRARY_PATH na
#              aktualni adresar - export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./



# prekladac
CC=gcc  
CPPC=g++

# povinne flagy								
CFLAGS=-std=c99 -pedantic -Wall -Wextra
CPPFLAGS=-std=c++11 -pedantic -Wall

# objektove soubory pro sestaveni knihoven
LIB_OBJ=hash_function.o htab_init.o htab_move.o htab_size.o htab_bucket_count.o htab_lookup_add.o htab_find.o htab_foreach.o htab_remove.o htab_clear.o htab_free.o

# *.c, *.cpp, *.h soubory
FILES=tail.c tail2.cpp hash_function.c htab_init.c htab_move.c htab_size.c htab_bucket_count.c htab_lookup_add.c htab_find.c htab_foreach.c htab_remove.c htab_clear.c htab_free.c io.c wordcount.c hash_table.h io.h



# $make
all: tail wordcount wordcount-dynamic tail2



# *.o soubory - tail, tail2
tail.o: tail.c
	$(CC) $(CFLAGS) -c tail.c -o tail.o

tail2.o: tail2.cpp
	$(CPPC) $(CPPFLAGS) -c tail2.cpp -o tail2.o



# *.o soubory - libhtab.a, libhtab.so
hash_function.o: hash_function.c
	$(CC) $(CFLAGS) -fPIC -c hash_function.c -o hash_function.o

htab_init.o: htab_init.c
	$(CC) $(CFLAGS) -fPIC -c htab_init.c -o htab_init.o

htab_move.o: htab_move.c
	$(CC) $(CFLAGS) -fPIC -c htab_move.c -o htab_move.o

htab_size.o: htab_size.c
	$(CC) $(CFLAGS) -fPIC -c htab_size.c -o htab_size.o

htab_bucket_count.o: htab_bucket_count.c
	$(CC) $(CFLAGS) -fPIC -c htab_bucket_count.c -o htab_bucket_count.o 

htab_lookup_add.o: htab_lookup_add.c
	$(CC) $(CFLAGS) -fPIC -c htab_lookup_add.c -o htab_lookup_add.o

htab_find.o: htab_find.c
	$(CC) $(CFLAGS) -fPIC -c htab_find.c -o htab_find.o

htab_foreach.o: htab_foreach.c
	$(CC) $(CFLAGS) -fPIC -c htab_foreach.c -o htab_foreach.o

htab_remove.o: htab_remove.c
	$(CC) $(CFLAGS) -fPIC -c htab_remove.c -o htab_remove.o

htab_clear.o: htab_clear.c
	$(CC) $(CFLAGS) -fPIC -c htab_clear.c -o htab_clear.o

htab_free.o: htab_free.c
	$(CC) $(CFLAGS) -fPIC -c htab_free.c -o htab_free.o



# *.o soubory - wordcount
io.o: io.c
	$(CC) $(CFLAGS) -c io.c -o io.o

wordcount.o: wordcount.c
	$(CC) $(CFLAGS) -c wordcount.c -o wordcount.o



# knihovny
libhtab.a: $(LIB_OBJ)
	ar rcs libhtab.a $(LIB_OBJ)

libhtab.so: $(LIB_OBJ)
	gcc $(CFLAGS) -shared -fPIC $(LIB_OBJ) -o libhtab.so



# binarky
tail: tail.o
	$(CC) $(CFLAGS) tail.o -o tail

tail2: tail2.o
	$(CPPC) $(CPPFLAGS) tail2.cpp -o tail2

wordcount: wordcount.o io.o libhtab.a
	$(CC) $(CFLAGS) io.o wordcount.o libhtab.a -static -o wordcount

wordcount-dynamic: wordcount.o io.o libhtab.so
	$(CC) $(CFLAGS) -L. wordcount.o io.o libhtab.so -o wordcount-dynamic -lhtab



# $make pack
pack:
	zip xdusek27.zip $(FILES) Makefile



# $make clean-o
clean-o:
	rm -f *.o



# $make clean-all
clean-all:
	rm -f tail
	rm -f tail2
	rm -f wordcount
	rm -f wordcount-dynamic
	rm -f libhtab.a
	rm -f libhtab.so
	rm -f *.o
	rm -f xdusek27.zip