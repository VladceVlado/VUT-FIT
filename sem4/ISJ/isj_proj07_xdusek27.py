#!/usr/bin/env python3

# Sources:
# 	- Decorators - http://python-3-patterns-idioms-test.readthedocs.io/en/latest/PythonDecorators.html
# 	- Iterables / generators / yield - https://stackoverflow.com/questions/231767/what-does-the-yield-keyword-do
# 	- With statement - https://docs.python.org/3/reference/compound_stmts.html#the-with-statement

import sys

##########################################################################################################
# TASK 1

class TooManyCallsError(Exception):
	"""
		Auxiliary class for task1 for raising exception
	"""
	pass


class limit_calls(object):
	"""
		Decorator with arguments for calling functions with limit number of calls

	"""
	def __init__(self, max_calls = 2, error_message_tail = "called too often"):
		"""
			Initialization of decorator's arguments
        """
		self.max_calls = max_calls
		self.error_message_tail = error_message_tail
		self.calls = 0

	def __call__(self, fnc):
		"""
			Initialization of function object.
		"""
		def wrapped_fnc(*args):
			"""
				Wrapped function, if the function fnc was called so many times raise exception
			"""
			self.calls += 1
			if self.calls > self.max_calls:
				raise TooManyCallsError('function \"' + fnc.__name__ + '\" - ' + self.error_message_tail)
			result = fnc(*args)
			return result

		return wrapped_fnc

##########################################################################################################
# TASK 2

def ordered_merge(*args, selector = []):
	"""
		Go through iterable objects *args and select items according to selector
	"""
	generator_list = []

	try:
		for elem in args:
			generator_list.append(iter(elem))
	except TypeError:
		sys.stderr.write("Object must be a collection object which supports the iteration protocol.\n")
		return

	for i in selector:
		yield(next(generator_list[i]))

##########################################################################################################
# TASK 3

class Log(object):
	"""
		Write into file
		Use 'With Statement Context Manager' ('try…except…finally' encapsulation)
	"""
	def __init__(self, file_name):
		"""
			Initialization of file name
		"""
		self.file_name = file_name

	def __enter__(self):
		"""
			Open the file and first write
		"""
		self.fr = open(self.file_name, 'w+')
		self.fr.write("Begin\n")
		return self

	def logging(self, text):
		"""
			Write to the file
		"""
		self.fr.write(text + "\n")

	def __exit__(self, exc_type, exc_val, exc_tb):
		"""
			Last write and close the file
		"""
		self.fr.write("End\n")
		self.fr.close()
		return True

##########################################################################################################
# TESTS

def test_task1():
	"""
		Testing functionality of task1
	"""
	print("##### TASK 1 - TEST 1 #####")

	@limit_calls(3, "That's too much.")
	def pyth(a, b):
		return a**2 + b**2

	print(pyth(3, 4))
	print(pyth(6, 8))
	print(pyth(7, 6))

	print()

	print("##### TASK 1 - TEST 2 #####")

	@limit_calls(1, "That's too much.")
	def pyth2(a, b):
		return a**2 + b**2

	print(pyth2(3, 4))
	try:
		print(pyth2(6, 8))
	except TooManyCallsError:
		print("TooManyCallsError catched")

	print()


def test_task2():
	"""
		Testing functionality of task2
	"""
	print("##### TASK 2 - TEST 1 #####")
	print(list(ordered_merge('abcde', [1, 2, 3], (3.0, 3.14, 3.141), range(11, 44, 11), selector = [2,3,0,1,3,1])))
	print()

	print("##### TASK 2 - TEST 2 #####")
	print(list(ordered_merge(123, [1, 2, 3], (3.0, 3.14, 3.141), range(11, 44, 11), selector = [2,3,0,1,3,1])))
	print()


def test_task3():
	"""
		Testing functionality of task3
	"""
	print("##### TASK 3 - TEST 1 #####")

	with Log('mylog.txt') as logfile:
		logfile.logging('Test1')
		logfile.logging('Test2')
		a = 1/0
		logfile.logging('Test3')

	print("Things was written into mylog.txt")


if __name__ == '__main__':
	test_task1()
	test_task2()
	test_task3()
