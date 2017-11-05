import sys
import os

filename  = 'Input/fixedTest.txt'
with open(filename, 'w') as outfile:
	with open('Input/test2.txt') as infile:
		for line in infile:
			numbers = line.split(' ')
			numbers[0] = int(numbers[0]) - 1;
			numbers[1] = int(numbers[1]) - 1;
			outfile.write(str(numbers[0]) + " " + str(numbers[1]) + "\n")