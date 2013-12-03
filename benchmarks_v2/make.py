#!/usr/bin/env python

import subprocess
from os import listdir, system
from os.path import join

srcDir = "src/"
compDir = "compiled/"
srcFiles = listdir(srcDir)
srcFiles = filter(lambda x: '~' not in x, srcFiles) #remove temporary files from list
optFile = "optimizations.txt"

with open(optFile, 'r') as f:
	optimizations = [line.strip() for line in f]
optimizations.append('')

success = True
errors = set([])
for srcName in srcFiles:
	path = join(srcDir, srcName)
	if '.cpp' in srcName:
		noExt = srcName[:-4]
	elif '.c' in srcName:
		noExt = srcName[:-2]
	for opt in optimizations:
		cmd = "g++ " + path + " -o " + compDir + noExt + opt + " " + opt
		print cmd
		output = system(cmd)
		if output != 0:
			print "compile error with %s" % srcName
			errors.add(srcName)
			success = False

print optimizations
if success:
	print "All compilation completed successfully"
else:
	print "compile errors with %s" % errors
