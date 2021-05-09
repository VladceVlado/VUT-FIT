# VUT FIT MITAI
# KRY 2021/2022
# Project: Elliptic-curve cryptography
# Author: Vladimir Dusek
# Login: xdusek27

# Usage:
#   $ make decipher publicKey="(0x477...3e, 0xaa0...dc)"
#   $ make pack

EXE = main.py
SRC = elliptic_curve.py main.py point.py utils.py
PACK = xdusek27.zip

.PHONY: decipher

decipher:
	./$(EXE) "$(publicKey)"

pack:
	@zip $(PACK) $(SRC) Makefile
