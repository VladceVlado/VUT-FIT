# VUT FIT MITAI
# KRY 2020/2021
# Project 1: Vigenere cipher
# Author: Vladimir Dusek
# Login: xdusek27

# Usage:
#   $ make              # Compile project
#   $ make clean        # Remove object files and deplist
#   $ make clean-all    # Remove object files, deplist and binaries

RC = rustc
SRC = main.rs
EXEC = kry

.PHONY: all run clean clean-all

all:
	$(RC) $(SRC) -o $(EXEC)

run: all
	./$(EXEC)

clean:
	rm -f $(EXEC)
