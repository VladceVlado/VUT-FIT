# Usage:
#   $ make
#   $ make clean
#   $ make clean-all

PROJ = itu-tr

.PHONY: all clean clean-all

all:
	pdflatex $(PROJ).tex
	pdflatex $(PROJ).tex

clean:
	-rm -f $(PROJ).aux $(PROJ).log $(PROJ).toc

clean-all:
	-rm -f $(PROJ).aux $(PROJ).log $(PROJ).toc $(PROJ).pdf
