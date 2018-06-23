# VUT FIT
# IPK 2017/2018
# Project 1
# Variant 1: Klient-server pro ziskani informace o uzivatelich
# Author: Vladimir Dusek, xdusek27 (2BIT)
# Date: 12/3/2018
# File: Makefile

# Usage:
#   $ make
#   $ make clean
#   $ make clean-all
#   $ make pack

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic
OBJFILES = client.o server.o error.o
SERVER_EXE = ipk-server
CLIENT_EXE = ipk-client
ARCHIVE = xdusek27.zip

%.o : %.c
	$(CC) $(CFLAGS) -c $<

all: $(SERVER_EXE) $(CLIENT_EXE)

server.o: server.c server.h error.h
client.o: client.c client.h error.h
error.o: error.c error.h

$(SERVER_EXE): server.o error.o
	$(CC) $(CFLAGS) server.o error.o -o $@

$(CLIENT_EXE): client.o error.o
	$(CC) $(CFLAGS) client.o error.o -o $@

.PHONY: clean clean-all pack

clean:
	rm -f $(OBJFILES)

clean-all:
	rm -f $(OBJFILES) $(SERVER_EXE) $(CLIENT_EXE) $(ARCHIVE)

pack:
	zip $(ARCHIVE) client.c client.h server.c server.h error.c error.h dokumentace.pdf Makefile
