# VUT FIT 2BIT
# IDS 2017/2018
# Semester project
# Author: Vladimir Dusek, xdusek27
# Date: 2/5/2018
# File: Makefile (for task)

# Usage:
#   $ make
#   $ make clean
#   $ make clean-all

TASK = ids-task

.PHONY: all clean clean-all

all:
	pdflatex $(TASK).tex
	pdflatex $(TASK).tex

clean:
	-rm -f $(TASK).aux $(TASK).log

clean-all:
	-rm -f $(TASK).aux $(TASK).log $(TASK).pdf
