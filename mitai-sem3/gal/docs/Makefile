# VUT FIT MITAI
# GAL 2020/2021
# Project: Topic 38 - Graph Radio Coloring Parallelization
# Authors: Vladimir Dusek, Patrik Goldschmidt

# Usage:
#   $ make
#   $ make pdf
#   $ make vlna
#   $ make clean
#   $ make clean-all

NAME = docs

.PHONY: all pdf vlna clean clean-all

all:
	pdflatex $(NAME).tex
	pdflatex $(NAME).tex

pdf: all clean

vlna:
	@vlna -l $(NAME).tex

clean:
	@rm -f $(NAME).aux $(NAME).fdb_latexmk $(NAME).fls $(NAME).log $(NAME).out $(NAME).synctex.gz $(NAME).te~

clean-all: clean
	rm -f $(NAME).pdf
