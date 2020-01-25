# VUT FIT 1MITAI
# MSP 2019/2020
# Project
# Author: Vladimir Dusek, xdusek27
# Date: 26/11/2019
# File: Makefile

# Usage:
#   $ make
#   $ make pdf
#   $ make vlna
#   $ make clean
#   $ make clean-all

NAME = xdusek27

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
