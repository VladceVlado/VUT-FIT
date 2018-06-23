#!/usr/bin/env python3

import types
import itertools
import re


def first_nonrepeating(string):
	"""
		Return first non repeating character in the string
	"""
	if not isinstance(string, str):
		return None

	string = re.sub(r'\s+', '', string)

	if string == "":
		return None

	order = []
	counts = {}

	for char in string:
		if char in counts:
			counts[char] += 1
		else:
			counts[char] = 1
			order.append(char)

	for char in order:
		if counts[char] == 1:
			return char

	return None


def combine4(square, expRes):
	"""
		Return list of unique solutions from tuple of 4 elements
		to expRes using +, -, *, /
	"""
	if not (isinstance(square, tuple) or isinstance(square, list)):
		return list()

	if len(square) != 4:
		return list()

	for elem in square:
		if isinstance(elem, int):
			if elem < 0:
				return list()
		else:
			return list()

	opers = ['+', '-', '*', '/']
	testRes = [''] * 11
	retRes = list()

	for i in opers:
		for j in opers:
			for k in opers:
				for l in itertools.permutations(square):
					testRes[0]  = "%d%s%d%s%d%s%d"     % (l[0], i, l[1], j, l[2], k, l[3])
					testRes[1]  = "(%d%s%d)%s%d%s%d"   % (l[0], i, l[1], j, l[2], k, l[3])
					testRes[2]  = "%d%s(%d%s%d)%s%d"   % (l[0], i, l[1], j, l[2], k, l[3])
					testRes[3]  = "%d%s%d%s(%d%s%d)"   % (l[0], i, l[1], j, l[2], k, l[3])
					testRes[4]  = "(%d%s%d)%s(%d%s%d)" % (l[0], i, l[1], j, l[2], k, l[3])
					testRes[5]  = "(%d%s%d%s%d)%s%d"   % (l[0], i, l[1], j, l[2], k, l[3])
					testRes[6]  = "%d%s(%d%s%d%s%d)"   % (l[0], i, l[1], j, l[2], k, l[3])
					testRes[7]  = "((%d%s%d)%s%d)%s%d" % (l[0], i, l[1], j, l[2], k, l[3])
					testRes[8]  = "(%d%s(%d%s%d))%s%d" % (l[0], i, l[1], j, l[2], k, l[3])
					testRes[9]  = "%d%s((%d%s%d)%s%d)" % (l[0], i, l[1], j, l[2], k, l[3])
					testRes[10] = "%d%s(%d%s(%d%s%d))" % (l[0], i, l[1], j, l[2], k, l[3])

					for result in testRes:
						try:
							if eval(result) == expRes:
								retRes.append(result)
						except ZeroDivisionError:
							pass

	return list(set(retRes))


def test():
	"""
		Testing functionality
	"""
	assert first_nonrepeating("tooth") == 'h'
	assert first_nonrepeating("lool") == None
	assert first_nonrepeating("aabccbdcbe") == 'd'
	assert first_nonrepeating("Roman Fursenko") == 'R'
	assert first_nonrepeating("it is a string") == 'a'
	assert first_nonrepeating('\t') == None
	assert first_nonrepeating('') == None
	first_nonrepeating(65)
	first_nonrepeating([6, 5])
	first_nonrepeating((6, 5, 6))
	first_nonrepeating(('gdg', 'sad'))
	first_nonrepeating({6, 5, 6})
	first_nonrepeating({'name': 'john', 'code': 6734})

	assert combine4([6, 6, 5, 2], 25) == []
	assert combine4([6, 6, 5, 2], -25) == []
	assert combine4([1, 1, 1, 1], 36) == []
	combine4([],36)
	combine4([6],36)
	combine4([6,6],36)
	combine4([6,6,5],36)
	combine4((1, 2, 3, 4), 32)
	combine4((5, 6, 7, 8), 20)
	combine4([6, 6, 5, 2], 36)
	combine4([6, 6, 5, 2], -36)
	combine4([1, 1, 1, 1], 0)
	combine4([6, 6, 5, 2], 17)
	combine4([6, 6, 5, 2], -16)


if __name__ == '__main__':
	test()
