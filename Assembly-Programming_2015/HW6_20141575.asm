TITLE MergeSort (HW6_20141575.asm)
INCLUDE Irvine32.inc
Merge PROTO
.data
str1 BYTE "The 1st string<<=24> : ",0
str2 BYTE "The 2nd string<<=24> : ",0
str3 BYTE "Merged string : ",0 
str4 BYTE "Bye!",0
arr1 BYTE 30 DUP(?)
arr2 BYTE 30 DUP(?)
arrsize1 DWORD ?
arrsize2 DWORD ?

.code
main PROC
L1 :
	mov edx,OFFSET str1    
	call WriteString

	mov ecx,26
	mov edx,OFFSET arr1
	call ReadString		    ;string�� �Է¹���
	
	cmp eax,0
	je END1				    ;string�� ���̰� 0�϶� bye ��� �� ����
	
	mov ecx,eax			    ; ecx�� ��Ʈ���� ���̸� ����
	mov arrsize1,eax		;arrsize1�� string�� ���̸� ����
	mov esi,OFFSET arr1     ;esi�� ��Ʈ���� ���� �ּҸ� ����
	call Valid_Check		;string�� valid ���� üũ
	
	cmp edx,0				;string�� size�� 24���ں��� Ŭ�� �ٽ� �Է�
	je L1
	

	mov ecx,eax				;ecx�� ��Ʈ���� ���̸� ����
	mov esi,OFFSET arr1		;esi�� ��Ʈ���� ���� �ּҸ� ����
	call Check_Sorted		;non decreasing order�� �Ǿ��ִ��� üũ	

	cmp eax,0				;������ ������ ������� L1���� ���ư�
	je L1

L2 :
	mov edx,OFFSET str2
	call WriteString

	mov ecx,26
	mov edx,OFFSET arr2     ;string�� �Է� ����
	call ReadString
	
	mov ecx,eax				;ecx�� string�� ���̸� ����
	mov arrsize2,eax
	mov esi,OFFSET arr2     ;esi�� ��Ʈ���� ���� �ּҸ� ����
	call Valid_Check 
	cmp edx,0
	je L2

	mov ecx,eax
	mov esi,OFFSET arr2
	call Check_Sorted
	
	cmp eax,0
	je L2
	 
	mov ecx,arrsize1
	add ecx,arrsize2   ;��ü string���� ����
	mov esi,OFFSET arr1     ;esi�� ù��° string�� ����
	mov edi,OFFSET arr2		;edi�� �ι�° string�� ����
	
	mov edx,OFFSET str3		
	call WriteString
	INVOKE Merge
	jmp L1
END1 : 
		mov edx,OFFSET str4
		call WriteString
		exit
main ENDP

Valid_Check PROC
;input : ecx = string size (excluding EOS 0)
;		 esi = start of the string
;output : edx = 1 if valid, 0 otherwise
	cmp ecx,25              ;string�� size�� 25���� ũ�ų������� ����(24���� ũ��)
	jae V3 
	V1 : 
		mov bl,BYTE PTR[esi] ;��Ʈ���� �ּҰ��� bl�� �����ϰ� ��
		cmp bl,'z'			 ;z���� Ŭ ��� ����
		ja V3
		cmp bl,'0'			 ;0���� ���� ��� ����
		jb V3
		cmp bl,'9'			 ;9���� �۰ų� ������� ���� �ε��� ��
		jbe V2
		cmp bl,'a'			 ;a���� ũ�ų� ������� ���� �ε��� ��
		jae V2
		cmp bl,'A'			;A���� ������ ����
		jb V3
		cmp bl,'Z'			;Z���� ũ�� ����
		ja V3

	V2 :
		inc esi             ;�ε�������
		dec ecx				;ecx ����
		cmp ecx,0			;ecx�� 0�϶� edx�� 1�� ����
		je V4
		jmp V1
	V3 :
		mov edx,0
		ret	
	V4 :
		mov edx,1
		ret
Valid_Check ENDP

Check_Sorted PROC
;input : ecx = string size(excluding EOS 0)
;		 esi = start of the string
;output : eax = 1 if non-decreasing, 0 otherwise

	dec ecx					       ;string size��ŭ for loop���ֱ� ����
	C1 :
		mov bl,BYTE PTR [esi]     
		cmp bl,BYTE PTR [esi+1]   ;������ ���ؼ� ���� ��ũ�� ����
		ja C2
		inc esi
		Loop C1

		mov eax,1
		ret
	C2 :
		mov eax,0
		ret
Check_Sorted ENDP

Merge PROC
;input : ecx = string size
;	   : esi = start of the string arr1
;	   : edi = start of the string arr2 
;output : X (print the merged string in Merge function)
	mov eax,0	;�ʱ�ȭ
	mov ebx,0
	mov edx,0
	M1:
		mov al,[esi]    ;�ѱ��ھ� al�� ����
		mov bl,[edi]	;�ѱ��ھ� bl�� ����
		cmp eax,0		;ù��° string�� ���������� ���� ��� �ι�° string�� �״�� �ڿ� �ٿ� �־���
		je M3			
		cmp ebx,0		;�ι�° string�� ���������� ���� ��� ù��° string�� �״�� �ڿ� �ٿ� �־���	
		je M4
		cmp eax,ebx		;�� string�� ���ؼ� �����Ÿ� ������ش�.
		ja M2		
		
		call WriteChar  ;�� ���� eax�� ����ϰ� ù��° ��Ʈ���� ���� �ε����� �ѱ�
		inc esi
		Loop M1
	M2:
		mov eax,ebx     ;�� ���� ebx�� ����ϰ� �ι�° ��Ʈ���� ���� �ε����� �ѱ�
		call WriteChar
		inc edi
		Loop M1
		call crlf
		ret
	M3:
		mov edx,edi
		call WriteString
		call crlf
		ret
	M4: 
		mov edx,esi
		call WriteString
		call crlf
		ret
Merge ENDP		
END MAIN