#!/usr/bin/env python3

# Task 1:
#   $ python3.6 -m cProfile lev.py > lev.cProfile_output
# 	- Number of function calls lev (ncalls for lev)

def lev(a, b):
	"""
		Recursively calculate the Levenshtein edit distance between two strings, a and b.
		Returns the edit distance.
	"""
	if ("" == a):
		return len(b) # returns if a is an empty string
	if ("" == b):
		return len(a) # returns if b is an empty string
	return min(lev(a[:-1], b[:-1]) + (a[-1] != b[-1]), lev(a[:-1], b) + 1, lev(a, b[:-1]) + 1)

print(lev('abracadabra', 'abba'))

# Editacni vzdalenosti muzeme merit vzdalenost, odlisnost, dvou retezcu.
# V pripade editacni vzdalenosti je vzdalenost definovana jako pocet operaci
# smazani znaku, vlozeni znaku a zameny znaku tak, aby se jeden retezec
# transformoval na druhy.
# Vypocet muzeme popsat nasledujicim zpusobem:
# Mame dany dva retezce a = a_1 a_2... a_m a b = b_1 b_2... b_n.
# Editacni vzdalenost d_i,j predpony delky i retezce a
# a predpony delky j retezce b muzeme vypocitat jako
# 1: d_i 1,j + 1 d d i,j = min i,j 1 + 1 (1) d i 1,j 1 + 1 pokud a i b j d i 1,j 1
# pokud a i = b j pro 1 i m a 1 j n.
# Prvni hodnota odpovida vymazani j-teho znaku z prvniho retezce, druha hodnota
# odpovida vlozeni znaku na (j+1)-ni pozici do prvniho retezce, treti hodnota
# odpovida vymene j-teho a i-teho znaku. Ctvrta hodnota se uplatni jen
# v pripade, ze jsou znaky v obou retezcich shodne.
# Na konci hodnota d_mn udava editacni vzdalenost retezcu a a b. Dale je
# pochopitelne definovano: d 0,0 = 0 d i,0 = i, pro 1 i m d 0,j = j, pro 1 j n Z
# predchoziho textu je patrne, ze cely vypocet muzeme implementovat jako
# rekurzivni funkci, kde vypocet hodnoty d mn se rozpadne na vypocet hodnot d m
# 1,n, d m,n 1, pripadne d m 1,n 1 a tak dale, az k definovanym koncovym hodnotam
# d 0,0 atd. Priklad Hodnota d ij udava editacni vzdalenost mezi prvnimi i znaky
# retezce a a prvnimi j znaky retezce b. Tuto hodnotu vsak neumime vypocitat
# primo, ale vypocteme ji na zaklade znalosti vzdalenosti mezi prvnimi i 1 znaky
# retezce a a prvnimi j 1 znaky retezce b. Timto zpusobem postupne redukujeme
# problem az na uroven pripadu, kdy je hodnota znama primo, napriklad pro d 0,0 =
# 0. Predpokladejme, ze a = abcabba a b = cbabac, odtud m = 7 a n = 6. Editacni
# vzdalenost techto dvou retezcu se bude pocitat jako hodnota d 7,6 d 7,6 = min d
# 6,6 + 1 d 7,5 + 1 d 6,5 + 1 protoze a 7 b 6 tj. a c Editacni vzdalenost celych
# retezcu a a b je jinak receno rovna editacni vzdalenosti prvnich 7 znaku
# retezce a a prvnich 6 znaku retezce b. Hodnotu d 6,6 1 Predponou delky i
# retezce a se mysli prvnich i znaku od zacatku retezce. 9
