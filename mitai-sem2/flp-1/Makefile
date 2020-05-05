# VUT FIT MITAI
# FLP 2019/2020
# Funcional project
# Variant: plg-2-nka
# Author: Vladimir Dusek
# Login: xdusek27

SRC = src/main.hs src/grammar.hs src/automaton.hs src/utils.hs
EXE = plg-2-nka
TEST = test.sh
PACK = flp-fun-xdusek27.zip

.PHONY: all run test clean zip

all:
	ghc --make $(SRC) -o $(EXE)

run: all
	./$(EXE) $(opt) $(file)

test:
	./$(TEST)

clean:
	@rm -f src/*.hi src/*.o $(EXE) $(PACK)

zip:
	@zip -r $(PACK) $(SRC) Makefile $(TEST) test/ readme.txt
