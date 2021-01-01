# VUT FIT MITAI
# GAL 2020/2021
# Project: Topic 38 - Graph Radio Coloring Parallelization
# Authors: Vladimir Dusek, Patrik Goldschmidt

NAME=sequential

.PHONY: all run run-py clean

all:
	g++ -std=c++20 -Wall -Wextra $(NAME).cpp -o $(NAME)

run: all
	./$(NAME)

run-py:
	./$(NAME).py

clean:
	rm --force $(NAME)
