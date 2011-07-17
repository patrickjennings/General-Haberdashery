#!/usr/bin/python

import random

numbers = 10
numerators = []
denominators = []

for a in range(numbers):
	numerators.append(random.randint(10,1000))
	denominators.append(random.randint(2,9))

for a in range(numbers):
	print '{0}/{1}'.format(numerators[a], denominators[a])

raw_input("Press Enter for solutions...")

solutions = [(float(x) / float(y)) for (x, y) in zip(numerators,denominators)]

for a in range(numbers):
	print '{0}'.format(solutions[a])
