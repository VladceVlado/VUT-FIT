# VUT FIT
# IPK 2017/2018
# Project 1
# Variant 1: Klient-server pro ziskani informace o uzivatelich
# Author: Vladimir Dusek, xdusek27 (2BIT)
# Date: 12/3/2018
# File: Makefile (for task)

# Usage:
#   $ make
#   $ make clean
#   $ make clean-all

TASK = ipk1-task

all:
	pdflatex $(TASK).tex
	pdflatex $(TASK).tex

.PHONY: clean clean-all

clean:
	-rm -f $(TASK).aux $(TASK).log

clean-all:
	-rm -f $(TASK).aux $(TASK).log $(TASK).pdf
