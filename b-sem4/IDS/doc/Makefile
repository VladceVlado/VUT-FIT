# VUT FIT 2BIT
# IDS 2017/2018
# Semester project
# Author: Vladimir Dusek, xdusek27
# Date: 2/5/2018
# File: Makefile (for documentation)

# Usage:
#   $ make
#   $ make clean
#   $ make clean-all

DOC = ids-doc

.PHONY: all clean clean-all

all:
	pdflatex $(DOC).tex
	pdflatex $(DOC).tex

clean:
	-rm -f $(DOC).aux $(DOC).log $(DOC).toc

clean-all:
	-rm -f $(DOC).aux $(DOC).log $(DOC).toc $(DOC).pdf
