                                Public transportation integration fee settlement System
        
**Design Objective**
        
       In seoul, many public transportions are develop. Seoul use transportation card 
       
       and accomplish Public transportation integration fee settlement System by transportation card.
       
       This system is a fare system that charges fares according to the total distance between transportation and transportation.

**Program Requirement** 
        - Use gcc compiler on Linux
        - Solve problem by using Linked list and Structure
        - Make 3 programs
        
        
**Program** 

    20141575_1.c
    
            Input : Customer ID / Transportation / Station ID / Entry and exit time / Distance / Amount
            
            Output : Print Customer ID and Balance, save balance to customer ID in customer.txt
   
    20141575_2.c
            
            Input : Customer ID / Transportation / Station ID / Entry&exit time / Transfer count / Entry&exit info
            
            Output : Save Customer ID, Amount, Balance, Distance
            
    20141575_3.c
   
            Input : Press 1 or 2 or 3
            
            Ouput : 
            
                    "1" : charging
                    
                            Input : 1 / Customer ID / Charging Amount
                            
                            Output : Amount Before Charging / Amount After Charging
                                     Update Amount in customer.txt
                    
                    "2" : Settlement
                            
                            Only for using credit card
                            
                            Output : Print customer who use credit card and Amount
                    
                    "3" : Last stop System
                    
                            input : Read data from reader_output.txt 
                            
                            Output : Print Amount at Entry&exit Station
                            
                          
                            
       
       
