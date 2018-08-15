TITLE Sogang Lottery    

INCLUDE Irvine32.inc

.data
str1 BYTE "Sogang Lottery!",0
str2 BYTE "Bye Lottery!",0
arr BYTE 6 DUP(0)
arrtemp BYTE 0
temp1 DWORD 0
temp2 DWORD 0

.code
main PROC
Start : 
	call Randomize
	mov edx,OFFSET str1
	call WriteString  ;sogang Lottery 를 출력
	call Crlf    

	push eax
	push ebx
	push ecx
	push edx  ;함수 진입하기전 레지스터들은 모두 stack에 저장하여 함수 진입전의 값을 유지해야한다.

	call CreateRandomNumber  ;첫번째 로또수를 레지스터 al에받음
	mov arr[0],al  ;첫번째 로또수를 arr[0]에 저장

	L1:  ;두번째 로또수를 arr[1]에저장
		call CreateRandomNumber  
		cmp arr[0],al   ;arr[0]과 al이 같을때 L1으로 돌아가고 다를경우 arr[1]에 al값을 저장
		je L1
		mov arr[1],al
	L2:  ;3번째 로또수
		call CreateRandomNumber
		cmp arr[0],al
		je L2
		cmp arr[1],al
		je L2
		mov arr[2],al
	L3:   ;4번째 로또수
		call CreateRandomNumber
		cmp arr[0],al
		je L3
		cmp arr[1],al
		je L3
		cmp arr[2],al
		je L3
		mov arr[3],al
	L4:  ;5번째 로또수
		call CreateRandomNumber
		cmp arr[0],al
		je L4
		cmp arr[1],al
		je L4
		cmp arr[2],al
		je L4
		cmp arr[3],al
		je L4
		mov arr[4],al
	L5:	;6번째 로또수
		call CreateRandomNumber
		cmp arr[0],al
		je L5
		cmp arr[1],al
		je L5
		cmp arr[2],al
		je L5
		cmp arr[3],al
		je L5
		cmp arr[4],al
		je L5
		mov arr[5],al
	
		;크기 순으로 배열들을 정렬
	mov ecx,6
	;for(i=5;i>0;i--)
	T1 :
		mov edx,ecx ; edx=i
		push ecx
		mov ecx,6		; j
		;for(j=6;j>i;j--)
		T2 :
			
			mov edi,ecx			; j = ecx 
			mov esi,edx			; i = edx
								
			dec esi				; i=  edx-1 = esi-1	
			dec edi				; j = ecx-1 = edi-1
			cmp ecx,edx			; ecx== edi(i==j) 일경우 loop밖으로나감
			je F1
			mov al,arr[esi]		;al=arr[i]
			mov bl,arr[edi]		;bl=arr[j]
			
			cmp bl,al		;arr[esi] arr[edi]를 비교해서 
			JAE F2			; >=
			xchg al,bl		; < 일경우에 두개를 바꿔줌
			F2 :
		
			mov arr[esi],al
			mov arr[edi],bl
	
		loop T2
		F1:
		pop ecx
	loop T1

; 출력 
 mov al,arr[0]
 call WriteDec
 call Crlf

 mov eax,500 ;0.5초마다출력
 call delay
 mov eax,0

 mov al,arr[1]
 call WriteDec
 call Crlf

 mov eax,500
 call delay
 mov eax,0

 mov al,arr[2]
 call WriteDec
 call Crlf

 mov eax,500
 call delay
 mov eax,0

 mov al,arr[3]
 call WriteDec
 call Crlf

 mov eax,500
 call delay
 mov eax,0

 mov al,arr[4]
 call WriteDec
 call Crlf

 mov al,arr[5]
 call WriteDec
 call Crlf

 pop edx
 pop ecx
 pop ebx
 pop eax

 call readchar
 

 cmp al,72h ;r입력시 재시작
 je Start


 mov edx,OFFSET str2 
 call WriteString
 call Crlf    

	exit
main ENDP

CreateRandomNumber PROC  ;1~45중 랜덤으로 한개를 return 하는 함수
	mov eax,45        
    call RandomRange     ;0~44중 한개를 가져오므로
	add eax,1          ;eax 에 1을 더해주어야 1~45중 한개를 return 해준다.
	ret	
CreateRandomNumber ENDP

END main