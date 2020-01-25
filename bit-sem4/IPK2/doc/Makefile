# VUT FIT
# IPK 2017/2018
# Project 2
# Variant 3: DNS Lookup nastroj
# Author: Vladimir Dusek, xdusek27 (2BIT)
# Date: 9/4/2018
# File: Makefile (for documentation)

# Usage:
#   $ make
#   $ make clean
#   $ make clean-all

DOC = ipk2-doc

.PHONY: all clean clean-all

all:
	pdflatex $(DOC).tex
	pdflatex $(DOC).tex

clean:
	-rm -f $(DOC).aux $(DOC).log $(DOC).toc

clean-all:
	-rm -f $(DOC).aux $(DOC).log $(DOC).toc $(DOC).pdf
