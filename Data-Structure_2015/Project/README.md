# Program name
SNI(Sogang Navigation & Introduction)

# Design objective & problem
We always had a problem in finding the fastest route within the Sogang University. 
Also, when we need to copy something, we have to look around all the buildings in order to find a copy machine. 
However, there is a solution. The SNI gives you high quality of campus life.
When you are about to go to different building, and if you don’t know the fastest route, use SNI!
First, input your location. Next, input your destination. Then the SNI will find you the fastest route.
When you need to go to computer lab, and if you don’t know exact buildings where the labs are in, you can use the second function of SNI.
If you input the facility you want, the SNI shows you building where facility is located. 
If you input the building’s name, the program shows you all the facilities in the building.

# Requirment
Search the facility(facility)	The program shows you the building where the facility is located.

Search the facility(building)	The program shows you all the facilities that are in the building.

Navigation 	Find the fastest route from start point to destination.

In main menu, 3 functions are represented. We used switch function. Thus, in case input is 1, call introduction() function. In case input is 2, call navigation() function. Last, in case input of 3, Program is finished. Until the user input the number 3, program is repeated. If the input is out of range(1~3), error message is printed. In case input is 1, introduction() function is called. When it is called, the program initializes the information of facilities and buildings by calling insertname() function and insertfacility() function. And it prints out 3 menu. If the input is 1, the program prints facility list, and the user inputs the facility user wants. Then, the program prints the location of facility, which is classed as buildings. If the input is 2, the program shows building list, and the user inputs the building user wants. Then, the program prints all the facilities that are int that building. If the input is 3, it goes back to main menu. We used switch function to constitute introduction() function.


If the input of main menu is 2, it calls navigation() function. It has 2 menus. First option finds the shortest way and time consumed, and the second option is going back to main menu. When the first option is performed, it shows the place point list and the user has to input the start point and the destination. We check the input by search(char*) function. If the start point and destination are different from saved information, it prints error message and goes back to menu. If the input is appropriate, it calls FindWay(start_pt, stop_pt) function and shows the fastest route. In Findway(start_pt, stop_pt) function, it calls wayinit() function to initialize road’s information and calls InitGraph() function and InitSogangMap() function. In InitGraph() function, adjacency matrix is initialized. In InitSogangMap() function, vertex is created by information of road’s information. Malloc is used to allocate memory, and FindShortesttime(cost) function is used to calculate shortest hour. Floyd-Warshall algorithm is used to find shortest hour. Then, the program shows this shortest hour and by using findFast(start, stop) function, previous passages are printed. Lastly, using free function to free the allocated memory.


# Teammates
Lim Hyung Suk(20141575), Kim Min Jung(20141511), Park Yeong Joon(20141525)

Contribution to all members 33.3% each.
