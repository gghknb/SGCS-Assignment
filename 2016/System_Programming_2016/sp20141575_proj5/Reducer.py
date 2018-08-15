#!/usr/bin/python
#-*- coding: utf-8 -*-

from sys import stdin, stdout

library = {}

while True:
	
	line = stdin.readline().strip()
	if line == "":
		break

	word, cnt = line.split('\t')

	if word in library :				#plus count of words
		library[word] += int(cnt)
	else :
		library[word] = int(cnt)
	

listkey = library.keys()
listkey.sort()
listkey.sort(key = str.upper)      # sort list

for word in listkey :				#print
	stdout.write(word + "\t" + str(library[word]) + "\n")
