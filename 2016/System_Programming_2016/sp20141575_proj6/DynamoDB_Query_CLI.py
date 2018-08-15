execfile('20141575.conf')

from sys import stdin,stdout
from boto.dynamodb2.table import Table

myTable = Table('project6')    #table name
while True:
	bigram = stdin.readline().strip()   #get input
	if bigram == "":		   #if input is nothing exit the program
		break;
	item = myTable.get_item(words = bigram)  #Primary key
	if item:				#if item exist, print counts, does not exist  print 0
		print item['counts']
	else: 
		print '0'
