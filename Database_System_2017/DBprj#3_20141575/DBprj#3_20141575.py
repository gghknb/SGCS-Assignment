import datetime
import time
import sys
import MeCab
from pymongo import MongoClient
import math as m
import operator
def printMenu() :
    print "1. WordCount"
    print "2. TF-IDF"
    print "3. Similarity"
    print "4. MorpAnalysis"
    print "5. CopyData"


def MorpAnalysis(docs,col_tfidf) :
    print "MorpAnalysis"
    t = MeCab.Tagger('-d/usr/local/lib/mecab/dic/mecab-ko-dic')

    stop_word = {}
    f = open("wordList.txt","r")
    while True:
        line = f.readline()
        if not line :
            break
        stop_word[line.strip('\n')] = line.strip('\n')
    f.close()
    
    for doc in docs :
        content = doc['content']
        nodes = t.parseToNode(content.encode('utf-8'))

        MorpList = []
        while nodes :
            if nodes.feature[0] == 'N' and nodes.feature[1] == 'N' :
                w = nodes.surface
                if not w in stop_word :
                    MorpList.append(w)
            nodes = nodes.next

        contentDic = {}
        for key in doc.keys() :
            contentDic[key] = doc[key]
        contentDic['morp'] = MorpList
        col_tfidf.update({'_id':contentDic['_id']},contentDic,True)
   
    docs.rewind()
    inputid = raw_input("Insert Object ID : ")
# print("morp : ")
    for doc in docs:
        if inputid == str(doc['_id']) :
           for distinct in doc['morp'] :
                print("\t" + distinct)  
           break
def WordCount(docs,col_tfidf):
    print "WordCount"
# inputid = input("Insert Object ID")

    for doc in docs :
        content = doc['morp']
        word_cnt = {}
        for word in content :
            if word in word_cnt.keys() :
                word_cnt[word] = word_cnt[word] + 1
            elif word not in word_cnt:
                word_cnt[word] = 1
        contentDic = {}
        for key in doc.keys() :
            contentDic[key] = doc[key]
        contentDic['Word_Count'] = word_cnt    
        col_tfidf.update({'_id':contentDic['_id']},contentDic,True)
    
    docs.rewind()
    inputid = raw_input("Insert Object ID : ")
#    print("Word_Count")
    for doc in docs:
        if inputid == str(doc['_id']) :
            for word in doc['Word_Count']:
                if len(word) >= 4 :
                    print "\t" + word +" : " ,
                    print (doc['Word_Count'][word])
                else :
                    print "\t" + word + "\t : ",
                    print(doc['Word_Count'][word])
            break

def TfIdf(docs, col_tfidf) :
    print "TF-IDF"
    
    #idf
    idf = {}
    doccnt = 0
    for doc in docs :
       temp = doc['Word_Count']
       doccnt += 1
       for word in temp :
           if word in idf :
                idf[word] = idf[word] + 1
           elif word not in idf :
                idf[word] = 1
        
    for word,cnt in idf.items() :
        idf[word] = m.log(float(doccnt)/float(cnt))
    
    docs.rewind()

    for doc in docs :
        #calculate tf
        tf = {}
        tfidf = {}
        total_word = doc['Word_Count']
        total_cnt = sum(total_word.values())
        #count total word
        for word in total_word :
            if word in tfidf.keys() :
                continue
            tf[word] = float(total_word[word]) / float(total_cnt)
#          tf = total_word[word] / total_cnt
            tfidf[word] = float(tf[word]) *  float(idf[word])

        contentDic = {}
        for key in doc.keys() :
            contentDic[key] = doc[key]
        contentDic['TFIDF'] = tfidf
        col_tfidf.update({'_id':contentDic['_id']},contentDic,True)
    
    docs.rewind()
    inputid = raw_input("Insert Object ID : ")
# print("TFIDF")
    cnt = 0
    for doc in docs:
        if inputid == str(doc['_id']) :
            sorted_tfidf = sorted(doc['TFIDF'].iteritems(), key = operator.itemgetter(1),reverse = True)
            for u,v in sorted_tfidf :
                if cnt == 10 :
                    break
                if len(u) >= 4 :
                    print u  + ":",
                    print v
                else : 
                    print u + "\t : ",        
                    print v
                cnt += 1
            break

def Similarity(docs,col_tfidf) :
    inputID1 = raw_input("Insert Object ID(1) : ")
    inputID2 = raw_input("Insert Object ID(2) : ")
    
    cnt1 = 0
    cnt2 = 0
    for doc in docs :
        if inputID1 == str(doc['_id']) :
            vectorA = doc['TFIDF']
            cnt1 = 1
        if inputID2 == str(doc['_id']) :
            vectorB = doc['TFIDF']
            cnt2 = 1
        if cnt1 == 1 and cnt2 == 1 :   
            break
       
    for wordA in vectorA :
            if wordA not in vectorB :
                       vectorB[wordA] = 0
    for wordB in vectorB :
            if wordB not in vectorA :
                       vectorA[wordB] = 0
    sumA = 0
    sumB = 0
    for wordA in vectorA :
        sumA += float(vectorA[wordA]) * float(vectorA[wordA])
    sumA = m.sqrt(sumA)
    for wordB in vectorB :
        sumB += float(vectorB[wordB]) * float(vectorB[wordB])
    sumB = m.sqrt(sumB)
    
    AB = 0
    for index in vectorA.keys() :
        AB += float(vectorA[index])*float(vectorB[index])

    similarity = float(AB)/float((sumA*sumB))
    print similarity

def copyData(docs,col_tfidf) :
    col_tfidf.drop()
    for doc in docs:
        contentDic = {}
        for key in doc.keys():
            if key != "_id" :
                contentDic[key] = doc[key]
        col_tfidf.insert(contentDic)



conn = MongoClient('localhost')
db = conn['db20141575']
db.authenticate('db20141575','db20141575')
col = db['news']
col_tfidf = db['news_tfidf']

printMenu()
selector = input()

docs = col_tfidf.find()
if selector == 1 :
    WordCount(docs,col_tfidf)
elif selector == 2:
    TfIdf(docs,col_tfidf)
elif selector == 3:
    Similarity(docs,col_tfidf)
elif selector == 4:
    MorpAnalysis(docs,col_tfidf)
elif selector == 5:
    docs = col.find()
    copyData(docs,col_tfidf)
