# VUT FIT MITAI
# GAL 2020/2021
# Project: Topic 38 - Graph Radio Coloring Parallelization
# Authors: Vladimir Dusek, Patrik Goldschmidt

# Usage:
#   $ make
#   $ make pdf
#   $ make clean
#   $ make clean-all

NAME = slides

.PHONY: all pdf clean clean-all

all:
	pdflatex $(NAME).tex
	pdflatex $(NAME).tex

pdf: all clean

clean:
	@rm -f $(NAME).dvi $(NAME).log $(NAME).blg $(NAME).bbl $(NAME).toc $(NAME).aux $(NAME).out $(NAME).lof $(NAME).ptc $(NAME).nav $(NAME).snm $(NAME).fls $(NAME).vrb $(NAME).fdb_latexmk __latexindent_temp.tex *.te~

clean-all: clean
	rm -f $(NAME).pdf
