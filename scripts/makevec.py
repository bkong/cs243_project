'''just a script for producing vectors'''

from random import random, randint

f = open('../data/vectorsFloat.txt', 'w')


for i in range(10**6):
	f.write(str(round(random(), 3)))
	f.write(' ')
	f.write(str(round(random(), 3)))
	f.write(' ')
	f.write(str(round(random(), 3)))
	f.write('\n')

'''
for i in range(10**6):
	f.write(str(randint(1,100)))
	f.write(' ')
	f.write(str(randint(1,100)))
	f.write(' ')
	f.write(str(randint(1,100)))
	f.write('\n')'''

f.close()
