#!/usr/bin/env python3


def can_be_a_set_member_or_frozenset(item):
	""" Task 1 - If item can be a member of set, return item, else return frozenset(item) """
	# Member of set cannot be 'unhashable type' (set, list, dict)
	s = set()
	# try to add item to the set
	try:
		s.add(item)
	# when 'TypeError' return frozen set, else return set
	except TypeError:
		return frozenset(item)
	else:
		return item

def all_subsets(lst):
	""" Task 2 - Return all subsets of given set represented as list """
	tmp = []
	result = [[]]
	# implementation of the algorithm described in the task
	for elem in lst:
		tmp = [last + [elem] for last in result]
		result = result + tmp
	return result

def all_subsets_excl_empty(*args, exclude_empty = True):
	""" Task 3 - Call all_subsets(), but members of set are given as arguments """
	if exclude_empty == True:
		# Without empty list
		return list(filter(None, all_subsets(args)))
	elif exclude_empty == False:
		# With empty list
		return all_subsets(args)


def test_tast1():
	assert can_be_a_set_member_or_frozenset(1) == 1
	assert can_be_a_set_member_or_frozenset('a') == 'a'
	assert can_be_a_set_member_or_frozenset("hell") == "hell"
	assert can_be_a_set_member_or_frozenset(3.14) == 3.14
	assert can_be_a_set_member_or_frozenset((1,2)) == (1,2)
	assert can_be_a_set_member_or_frozenset({"abc": 1, "de": 3}) == frozenset({"abc": 1, "de": 3})
	assert can_be_a_set_member_or_frozenset({'a', 'b'}) == frozenset({'a', 'b'})
	assert can_be_a_set_member_or_frozenset([1,2]) == frozenset([1,2])

def test_tast2():
	assert all_subsets(['a', 'b', 'c']) == [[], ['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]

def test_tast3():
	assert all_subsets_excl_empty('a', 'b', 'c') == [['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]
	assert all_subsets_excl_empty('a', 'b', 'c', exclude_empty = True) == [['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]
	assert all_subsets_excl_empty('a', 'b', 'c', exclude_empty = False) == [[], ['a'], ['b'], ['a', 'b'], ['c'], ['a', 'c'], ['b', 'c'], ['a', 'b', 'c']]

if __name__ == '__main__':
	test_tast1()
	test_tast2()
	test_tast3()
