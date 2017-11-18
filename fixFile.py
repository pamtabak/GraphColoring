import sys
import os
import re

filename  = 'Input/fixedTest.txt'
with open(filename, 'w') as outfile:
	with open('Input/david.txt') as infile:
		for line in infile:
			regex     = re.search('(\d+\s\d+)', line)
			regexLine = regex.group(0)
			numbers   = regexLine.split(' ')
			numbers[0] = int(numbers[0]) - 1;
			numbers[1] = int(numbers[1]) - 1;
			outfile.write(str(numbers[0]) + " " + str(numbers[1]) + "\n")