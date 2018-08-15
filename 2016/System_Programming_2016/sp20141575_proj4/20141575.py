import requests
from bs4 import BeautifulSoup


visited = []
cnt = 0    #page number
error = 0  #error flag 

def webcrawling(url,visited) :
	if url in visited :	  #if visited 			
		return 
	else :
			global cnt
			global error
			request = requests.get(url)
			soup = BeautifulSoup(request.content,"html.parser")
			visited.append(url)      	
			cnt = cnt + 1 
			outputfilename = "Output_" + "%04d" % cnt + ".txt"    #write a text to output file
			output = open(outputfilename,"w")
			output.writelines(soup.get_text())
			output.close()
			atag = soup.find_all('a')
			for urllink in atag : 
				nextlink = urllink.get('href')
				if nextlink != "" and '#' not in nextlink and '?' not in nextlink :
					if nextlink.find("http://cspro.sogang.ac.kr/~gr120160213/") == -1 :
						nextlink = "http://cspro.sogang.ac.kr/~gr120160213/" + nextlink
					try :
						req = requests.get(nextlink)
					except requests.exception.ConnectError:
						error = 1	
					if req.ok == True and error == 0 :    #if there does not exist error or request error, call function by reculsive
						webcrawling(nextlink,visited)
					

root = "http://cspro.sogang.ac.kr/~gr120160213/index.html"
webcrawling(root,visited)
URLwrite = open("URL.txt","w")		#write a URL to URL.txt file
length = len(visited)				#write URL
for i in range(length - 1) :              
	URLwrite.writelines(visited[i] + '\n')
URLwrite.writelines(visited[length -1])	#get rid of '\n'
URLwrite.close()


	

	
	

