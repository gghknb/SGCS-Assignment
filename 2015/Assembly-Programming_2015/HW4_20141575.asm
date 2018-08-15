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
	call WriteString  ;sogang Lottery �� ���
	call Crlf    

	push eax
	push ebx
	push ecx
	push edx  ;�Լ� �����ϱ��� �������͵��� ��� stack�� �����Ͽ� �Լ� �������� ���� �����ؾ��Ѵ�.

	call CreateRandomNumber  ;ù��° �ζǼ��� �������� al������
	mov arr[0],al  ;ù��° �ζǼ��� arr[0]�� ����

	L1:  ;�ι�° �ζǼ��� arr[1]������
		call CreateRandomNumber  
		cmp arr[0],al   ;arr[0]�� al�� ������ L1���� ���ư��� �ٸ���� arr[1]�� al���� ����
		je L1
		mov arr[1],al
	L2:  ;3��° �ζǼ�
		call CreateRandomNumber
		cmp arr[0],al
		je L2
		cmp arr[1],al
		je L2
		mov arr[2],al
	L3:   ;4��° �ζǼ�
		call CreateRandomNumber
		cmp arr[0],al
		je L3
		cmp arr[1],al
		je L3
		cmp arr[2],al
		je L3
		mov arr[3],al
	L4:  ;5��° �ζǼ�
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
	L5:	;6��° �ζǼ�
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
	
		;ũ�� ������ �迭���� ����
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
			cmp ecx,edx			; ecx== edi(i==j) �ϰ�� loop�����γ���
			je F1
			mov al,arr[esi]		;al=arr[i]
			mov bl,arr[edi]		;bl=arr[j]
			
			cmp bl,al		;arr[esi] arr[edi]�� ���ؼ� 
			JAE F2			; >=
			xchg al,bl		; < �ϰ�쿡 �ΰ��� �ٲ���
			F2 :
		
			mov arr[esi],al
			mov arr[edi],bl
	
		loop T2
		F1:
		pop ecx
	loop T1

; ��� 
 mov al,arr[0]
 call WriteDec
 call Crlf

 mov eax,500 ;0.5�ʸ������
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
 

 cmp al,72h ;r�Է½� �����
 je Start


 mov edx,OFFSET str2 
 call WriteString
 call Crlf    

	exit
main ENDP

CreateRandomNumber PROC  ;1~45�� �������� �Ѱ��� return �ϴ� �Լ�
	mov eax,45        
    call RandomRange     ;0~44�� �Ѱ��� �������Ƿ�
	add eax,1          ;eax �� 1�� �����־�� 1~45�� �Ѱ��� return ���ش�.
	ret	
CreateRandomNumber ENDP

END main