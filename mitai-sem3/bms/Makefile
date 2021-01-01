# VUT FIT MITAI
# BMS 2020/2021
# Project: Convolutional coder and Viterbi algorithm
# Author: Vladimir Dusek
# Login: xdusek27

SRC = bms.py
EXE = bms
TEST = test.sh
PACK = xdusek27.zip

.PHONY: all run test clean clean-all pack

all:
	@cp $(SRC) $(EXE)

run: all
	./$(EXE) --help

test:
	./$(TEST)

clean:
	@rm -f $(EXE)

clean-all:
	@rm -f $(EXE) $(PACK)

pack: all
	@zip $(PACK) $(SRC) Makefile
