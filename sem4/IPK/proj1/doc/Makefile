# VUT FIT
# IPK 2017/2018
# Project 1
# Variant 1: Klient-server pro ziskani informace o uzivatelich
# Author: Vladimir Dusek, xdusek27 (2BIT)
# Date: 12/3/2018
# File: Makefile (for documentation)

# Usage:
#   $ make
#   $ make clean
#   $ make clean-all

DOC = ipk1-doc

all:
	pdflatex $(DOC).tex
	pdflatex $(DOC).tex

.PHONY: clean clean-all

clean:
	-rm -f $(DOC).aux $(DOC).log $(DOC).toc

clean-all:
	-rm -f $(DOC).aux $(DOC).log $(DOC).toc $(DOC).pdf
