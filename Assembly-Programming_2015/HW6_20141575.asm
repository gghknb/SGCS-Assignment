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
	call ReadString		    ;string을 입력받음
	
	cmp eax,0
	je END1				    ;string의 길이가 0일때 bye 출력 후 종료
	
	mov ecx,eax			    ; ecx에 스트링의 길이를 저장
	mov arrsize1,eax		;arrsize1에 string의 길이를 저장
	mov esi,OFFSET arr1     ;esi에 스트링의 시작 주소를 저장
	call Valid_Check		;string이 valid 한지 체크
	
	cmp edx,0				;string의 size가 24글자보다 클때 다시 입력
	je L1
	

	mov ecx,eax				;ecx에 스트링의 길이를 저장
	mov esi,OFFSET arr1		;esi에 스트링의 시작 주소를 저장
	call Check_Sorted		;non decreasing order로 되어있는지 체크	

	cmp eax,0				;순서에 문제가 있을경우 L1으로 돌아감
	je L1

L2 :
	mov edx,OFFSET str2
	call WriteString

	mov ecx,26
	mov edx,OFFSET arr2     ;string을 입력 받음
	call ReadString
	
	mov ecx,eax				;ecx에 string의 길이를 저장
	mov arrsize2,eax
	mov esi,OFFSET arr2     ;esi에 스트링의 시작 주소를 저장
	call Valid_Check 
	cmp edx,0
	je L2

	mov ecx,eax
	mov esi,OFFSET arr2
	call Check_Sorted
	
	cmp eax,0
	je L2
	 
	mov ecx,arrsize1
	add ecx,arrsize2   ;전체 string길이 저장
	mov esi,OFFSET arr1     ;esi에 첫번째 string을 저장
	mov edi,OFFSET arr2		;edi에 두번째 string을 저장
	
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
	cmp ecx,25              ;string의 size가 25보다 크거나같으면 종료(24보다 크면)
	jae V3 
	V1 : 
		mov bl,BYTE PTR[esi] ;스트링의 주소값을 bl에 저장하고 비교
		cmp bl,'z'			 ;z보다 클 경우 종료
		ja V3
		cmp bl,'0'			 ;0보다 작을 경우 종료
		jb V3
		cmp bl,'9'			 ;9보다 작거나 같을경우 다음 인덱스 비교
		jbe V2
		cmp bl,'a'			 ;a보다 크거나 같을경우 다음 인덱스 비교
		jae V2
		cmp bl,'A'			;A보다 작으면 종료
		jb V3
		cmp bl,'Z'			;Z보다 크면 종료
		ja V3

	V2 :
		inc esi             ;인덱스증가
		dec ecx				;ecx 감소
		cmp ecx,0			;ecx가 0일때 edx를 1로 리턴
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

	dec ecx					       ;string size만큼 for loop해주기 위해
	C1 :
		mov bl,BYTE PTR [esi]     
		cmp bl,BYTE PTR [esi+1]   ;순서를 비교해서 앞이 더크면 종료
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
	mov eax,0	;초기화
	mov ebx,0
	mov edx,0
	M1:
		mov al,[esi]    ;한글자씩 al에 저장
		mov bl,[edi]	;한글자씩 bl에 저장
		cmp eax,0		;첫번째 string이 마지막까지 갔을 경우 두번째 string을 그대로 뒤에 붙여 넣어줌
		je M3			
		cmp ebx,0		;두번째 string이 마지막까지 갔을 경우 첫번째 string을 그대로 뒤에 붙여 넣어줌	
		je M4
		cmp eax,ebx		;두 string을 비교해서 작은거를 출력해준다.
		ja M2		
		
		call WriteChar  ;더 작은 eax를 출력하고 첫번째 스트링을 다음 인덱스로 넘김
		inc esi
		Loop M1
	M2:
		mov eax,ebx     ;더 작은 ebx를 출력하고 두번째 스트링을 다음 인덱스로 넘김
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