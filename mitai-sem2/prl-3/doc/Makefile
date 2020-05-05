# VUT FIT MITAI
# PRL 2019/2020
# Project 3: Viditelnost
# Author: Vladimir Dusek
# Login: xdusek27

# Usage:
#   $ make
#   $ make pdf
#   $ make vlna
#   $ make clean
#   $ make clean-all

NAME = doc

.PHONY: all pdf vlna clean clean-all

all:
	pdflatex $(NAME).tex
	pdflatex $(NAME).tex

pdf: all clean

vlna:
	vlna -l $(NAME).tex

clean:
	-rm -f $(NAME).aux $(NAME).log $(NAME).toc $(NAME).te~

clean-all:
	-rm -f $(NAME).aux $(NAME).log $(NAME).toc $(NAME).te~ $(NAME).pdf
