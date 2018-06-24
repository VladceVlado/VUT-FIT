# VUT FIT
# IPK 2017/2018
# Project 2
# Variant 3: DNS Lookup nastroj
# Author: Vladimir Dusek, xdusek27 (2BIT)
# Date: 9/4/2018
# File: Makefile (for task)

# Usage:
#   $ make
#   $ make clean
#   $ make clean-all

TASK = ipk2-task

.PHONY: all clean clean-all

all:
	pdflatex $(TASK).tex
	pdflatex $(TASK).tex

clean:
	-rm -f $(TASK).aux $(TASK).log $(TASK).toc

clean-all:
	-rm -f $(TASK).aux $(TASK).log $(TASK).toc $(TASK).pdf
