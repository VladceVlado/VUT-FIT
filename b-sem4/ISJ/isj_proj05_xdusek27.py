#!/usr/bin/env python3

import math


class Polynomial:
	""" Class for work with polynomials.
		Coefficients are saved as a list attribute (coeffs). """

	def __init__(self, *args, **kwargs):
		""" Saves coefficients of polynomial set as arguments to a self.coeffs list """
		self.coeffs = []
		if args:
			for elem in args:
				if isinstance(elem, list):
					self.coeffs = elem
					break
				else:
					self.coeffs.append(elem)
		if kwargs:
			iterator = 0
			for key, val in sorted(kwargs.items()):
				key = int(key[1:])
				while key != iterator:
					self.coeffs.append(0)
					iterator = iterator + 1
				self.coeffs.append(val)
				iterator = iterator + 1
		for elem in self.coeffs:
			if (self.coeffs[-1] == 0) and (len(self.coeffs) != 1):
				self.coeffs.pop()
			else:
				break

	def __str__(self):
		""" Conversion polynomial to a string """
		if all(i == 0 for i in self.coeffs):
			return "0"
		string = ""
		cnt = 0
		for elem in self.coeffs:
			if elem == 0:
				cnt = cnt + 1
				continue
			elif (elem == 1 or elem == -1) and cnt != 0:
				coeff = ""
			else:
				coeff = str(abs(elem))
			if cnt == len(self.coeffs) - 1:
				if elem < 0:
					sign = "- "
				else:
					sign = ""
			elif elem < 0:
				sign = " - "
			else:
				sign = " + "
			if cnt == 0:
				power = ""
			elif cnt == 1:
				power = "x"
			else:
				power = "x^" + str(cnt)
			string = sign + coeff + power + string
			cnt = cnt + 1
		return string

	def __eq__(self, other):
		""" Compare two polynomials """
		return self.coeffs == other.coeffs

	def __add__(self, other):
		""" Returns sum of polynomials """
		A = self.coeffs
		B = other.coeffs
		result = list()
		if len(A) > len(B):
			for i, b in enumerate(B):
				result.append(A[i] + b)
			i += 1
			while i < len(A):
				result.append(A[i])
				i += 1
		else:
			for i, a in enumerate(A):
				result.append(B[i] + a)
			i += 1
			while i < len(B):
				result.append(B[i])
				i += 1
		return str(Polynomial(result))

	def __pow__(self, exp):
		""" Returns polynomial power to the exp """
		power = int(exp)
		result = []
		for k in range(0, power + 1):
			nCr = math.factorial(power) // math.factorial(k) // math.factorial(power - k)
			result.append(nCr * ((self.coeffs[1]) ** (power - k)) * (self.coeffs[0] ** k))
		return Polynomial(result[::-1])

	def derivative(self):
		""" Returns derived polynomial """
		derivated = []
		idx = 1;
		for elem in self.coeffs[1:]:
			derivated.append(elem * idx)
			idx += 1
		return Polynomial(derivated)

	def at_value(self, *x):
		""" Returns value of polynomial at x,
			when there are 2 values, it returns their difference """
		x = list(x)
		if len(x) == 1:
			idx = 0
			result = 0
			for elem in self.coeffs:
				result += (x[0] ** idx) * elem
				idx += 1
			return result
		elif len(x) == 2:
			if x[0] > x[1]:
				return self.at_value(x[0]) - self.at_value(x[1])
			else:
				return self.at_value(x[1]) - self.at_value(x[0])


def test():
	assert str(Polynomial(0,1,0,-1,4,-2,0,1,3,0)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
	assert str(Polynomial([-5,1,0,-1,4,-2,0,1,3,0])) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x - 5"
	assert str(Polynomial(x7=1, x4=4, x8=3, x9=0, x0=0, x5=-2, x3= -1, x1=1)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
	assert str(Polynomial(x2=0)) == "0"
	assert str(Polynomial(x0=0)) == "0"
	assert Polynomial(x0=2, x1=0, x3=0, x2=3) == Polynomial(2,0,3)
	assert Polynomial(x2=0) == Polynomial(x0=0)
	assert str(Polynomial(x0=1)+Polynomial(x1=1)) == "x + 1"
	assert str(Polynomial([-1,1,1,0])+Polynomial(1,-1,1)) == "2x^2"
	pol1 = Polynomial(x2=3, x0=1)
	pol2 = Polynomial(x1=1, x3=0)
	assert str(pol1+pol2) == "3x^2 + x + 1"
	assert str(pol1+pol2) == "3x^2 + x + 1"
	assert str(Polynomial(x0=-1,x1=1)**1) == "x - 1"
	assert str(Polynomial(x0=-1,x1=1)**2) == "x^2 - 2x + 1"
	pol3 = Polynomial(x0=-1,x1=1)
	assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
	assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
	assert str(Polynomial(x0=2).derivative()) == "0"
	assert str(Polynomial(x3=2,x1=3,x0=2).derivative()) == "6x^2 + 3"
	assert str(Polynomial(x3=2,x1=3,x0=2).derivative().derivative()) == "12x"
	pol4 = Polynomial(x3=2,x1=3,x0=2)
	assert str(pol4.derivative()) == "6x^2 + 3"
	assert str(pol4.derivative()) == "6x^2 + 3"
	assert Polynomial(-2,3,4,-5).at_value(0) == -2
	assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3) == 20
	assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3,5) == 44
	pol5 = Polynomial([1,0,-2])
	assert pol5.at_value(-2.4) == -10.52
	assert pol5.at_value(-2.4) == -10.52
	assert pol5.at_value(-1,3.6) == -23.92
	assert pol5.at_value(-1,3.6) == -23.92

if __name__ == '__main__':
	test()