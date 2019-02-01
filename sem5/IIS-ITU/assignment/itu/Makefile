# VUT FIT 3BIT
# ITU 2018/2019
# Project
# Authors:
#     Vladimir Dusek, xdusek27
#     Petr Knetl, xknetl00
#     Martin Hosala, xhosal00
# Date: 29/9/2018
# File: Makefile

# Usage:
#   $ make
#   $ make clean
#   $ make clean-all

ASSIGNMENT = itu-assignment

.PHONY: all clean clean-all

all:
	pdflatex $(ASSIGNMENT).tex
	pdflatex $(ASSIGNMENT).tex

clean:
	-rm -f $(ASSIGNMENT).aux $(ASSIGNMENT).log

clean-all:
	-rm -f $(ASSIGNMENT).aux $(ASSIGNMENT).log $(ASSIGNMENT).pdf
