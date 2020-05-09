# VUT FIT MITAI
# SUR 2019/2020
# Project
# Authors: Vladimir Dusek, David Ling, Richard Mička, Tomáš Strych

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
