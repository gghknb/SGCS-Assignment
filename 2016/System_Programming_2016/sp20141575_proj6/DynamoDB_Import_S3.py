execfile('20141575.conf')

from boto.dynamodb2.table import Table
from boto.s3.connection import S3Connection

s3 = S3Connection()
bucket = s3.get_bucket('inputinputinputinput')   #bucket name inputinputinputinput

dynamoTable = Table('project6')		#table name project 6

temp = bucket.list() 			#get list of bucket

for i in temp:
	if 'Input' in i.key and 'part-' in i.key:
			content = i.get_contents_as_string()	#select input
			for line in content.split('\n'):    #read content
				if line.strip() != "":	     #not read null
					words, counts = line.split('\t')	#split by tab
					dynamoTable.put_item(data={'words':words, 'counts':counts})     # input data
