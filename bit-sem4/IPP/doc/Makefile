# VUT FIT
# IPP 2017/2018
# Project: Sada skriptu pro interpretaci nestrukturovaneho imperativniho jazyka IPPcode18
# Author: Vladimir Dusek, xdusek27 (2BIT)
# Date: 15/4/2018
# File: Makefile (for documentation)

# Usage
# 	- compile:    make
#   - clean:      make clean
#   - clean all:  make clean-all

DOC = ipp-doc

all:
	pdflatex $(DOC).tex
	pdflatex $(DOC).tex

clean:
	-rm -f $(DOC).aux $(DOC).log $(DOC).toc

clean-all:
	-rm -f $(DOC).aux $(DOC).log $(DOC).toc $(DOC).pdf
