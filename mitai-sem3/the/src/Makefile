# VUT FIT MITAI
# THE 2020/2021
# Project: ...
# Author: Vladimir Dusek

# Usage:
#   $ make
#   $ make pdf
#   $ make vlna
#   $ make clean
#   $ make clean-all

NAME = text

.PHONY: all pdf vlna clean clean-all

all:
	pdflatex $(NAME).tex
	pdflatex $(NAME).tex

pdf: all clean

vlna:
	@vlna -l $(NAME)*.tex

clean:
	@rm -f $(NAME).aux $(NAME).fdb_latexmk $(NAME).fls $(NAME).log $(NAME).out $(NAME).synctex.gz $(NAME)*.te~ __latexindent_temp.tex

clean-all: clean
	rm -f $(NAME).pdf
