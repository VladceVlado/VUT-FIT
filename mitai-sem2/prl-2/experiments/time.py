#!/usr/bin/env python3

from statistics import mean

def diff(first, second):
    second = set(second)
    return [item for item in first if item not in second]

dct = {}
for i in range(2, 26):
	dct[i] = []

fp = open('time.txt', 'r')

num = 2
iteration = 1

for line in fp:
	if line == '\n':
		num += 1
	elif iteration % 31 == 1:
		...
	else:
		dct[num].append(int(line))
	iteration += 1

print(dct)

for key, value in dct.items():
	print(key)
	print("max: " + str(max(value)))
	print("min: " + str(min(value)))
	dct[key] = diff(diff(dct[key], [max(value)]), [min(value)])
	dct[key] = diff(diff(dct[key], [max(value)]), [min(value)])

print(dct)

for key, value in dct.items():
	dct[key] = round(mean(value))

print(dct)
