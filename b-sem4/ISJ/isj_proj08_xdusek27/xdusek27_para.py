#!/usr/bin/env python3

# Task 3
# 	$ echo "GIL" > why.txt
# 	- Global Interpreter Lock (GIL)
# 	- Parallel programs using threads are not faster than normal sequential.
#
# Sources:
#	- https://stackoverflow.com/questions/10154487/how-to-get-a-faster-speed-when-using-multi-threading-in-python
#	- http://www.dabeaz.com/python/UnderstandingGIL.pdf

from multiprocessing import Process

def count(n):
    while n > 0:
        n -= 1

p1 = Process(target = count, args = (10 ** 8,))
p1.start()
p2 = Process(target = count, args = (10 ** 8,))
p2.start()
p1.join(); p2.join()
