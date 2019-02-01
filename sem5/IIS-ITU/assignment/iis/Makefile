# VUT FIT 3BIT
# IIS 2018/2019
# Semester project - Information System
# Authors:
#     Vladimir Dusek, xdusek27
#     Tomas Kukan, xkukan00
# Date: 26/9/2018
# File: Makefile

# Usage:
#   $ make
#   $ make clean
#   $ make clean-all

ASSIGNMENT = iis-assignment

.PHONY: all clean clean-all

all:
	pdflatex $(ASSIGNMENT).tex
	pdflatex $(ASSIGNMENT).tex

clean:
	-rm -f $(ASSIGNMENT).aux $(ASSIGNMENT).log

clean-all:
	-rm -f $(ASSIGNMENT).aux $(ASSIGNMENT).log $(ASSIGNMENT).pdf
