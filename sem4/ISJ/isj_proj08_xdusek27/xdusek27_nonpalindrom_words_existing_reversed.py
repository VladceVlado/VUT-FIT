#!/usr/bin/env python3

# Task 2:
#   $ { time ./xdusek27_nonpalindrom_words_existing_reversed.py < words.txt ; } 2> words.time_output
# 	- Edit the script to reduce running time as much as possible

import fileinput

words_reversed = set()
result = []

for line in fileinput.input():
	word = line.rstrip()
	word_rev = word[::-1]
	if not word == word_rev:
		words_reversed.add(word_rev)
		if word in words_reversed:
			result.append(word)
			result.append(word_rev)

print(sorted(result))
