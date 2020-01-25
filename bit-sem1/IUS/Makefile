# VUT FIT
# IUS 2016/2017
# Projekt - Model informacniho systemu
# Soubor: Makefile
# Autor: Vladimir Dusek, xdusek27 (1BIT)
# Datum: 27. 11. 2016

# Usage:
#   $ make
#   $ make clean
#   $ make clean-all

NAME = model_informacniho_systemu

.PHONY: all clean clean-all

all:
	pdflatex $(NAME).tex
	pdflatex $(NAME).tex

clean:
	-rm -f $(NAME).aux $(NAME).log $(NAME).toc

clean-all:
	-rm -f $(NAME).aux $(NAME).log $(NAME).toc $(NAME).pdf
