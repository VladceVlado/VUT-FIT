# Author: Vladimir Dusek
# Date: 10/12/2018

# Usage:
#   $ make
#   $ make pdf
#   $ make vlna
#   $ make clean
#   $ make clean-all

NAME = assignment

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
