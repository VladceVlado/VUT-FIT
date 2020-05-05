# VUT FIT MITAI
# FLP 2019/2020
# Logic project
# Variant: Turing machine
# Author: Vladimir Dusek
# Login: xdusek27

SRC = flp20-log.pl
EXE = flp20-log
TEST = test.sh
ZIP = flp-log-xdusek27.zip

.PHONY: all run test clean zip

all:
	swipl -q -g start -o $(EXE) -c $(SRC)

run: all
	./$(EXE)

test:
	./$(TEST)

clean:
	@rm -f $(EXE) $(ZIP)

zip:
	@zip -r $(ZIP) $(SRC) Makefile $(TEST) test/ readme.txt
