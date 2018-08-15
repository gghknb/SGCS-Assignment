#!/usr/bin/python
#-*- coding: utf-8 -*-

from sys import stdin, stdout

while True :
	string = stdin.readline().strip()           #input string
	if string =="":
		break
	string = string.lower()                     #change capital letter to small letter

	data = string.split()						#split data
	for i in range(0,len(data) -1) :								#print
		stdout.write(data[i] + ' ' + data[i+1] + "\t1\n")

