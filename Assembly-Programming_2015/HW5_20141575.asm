TITLE recursive (HW5_20141575.asm)

INCLUDE Irvine32.inc

.data
INCLUDE HW5_include.inc

.code
main PROC
	mov eax,0
	mov ecx,31				 ;bit의 string은 31을 넘지 않으니 ecx를 31로 설정
	mov esi,OFFSET stringEx1 ;string의 주소값을 저장
	call Checkstring		 ; string에서 1의 개수를 check해주는 checkstring함수 호출
	call Sumn				 ; 1부터 n까지 더해주는 함수 호출
	mov eax,ecx
	call WriteDec
	call crlf
	exit
main ENDP

Checkstring PROC
L1:
	mov ebx,[esi]  ;ebx에 string의 첫번째 문자가 저장
	inc esi		   
	cmp bl,48	;'0'과 비교
	jb L2		;'0'보다 작으면 L2로 이동
	cmp bl,49	;'1'과비교
	ja L2		;'1'보다 크면 L2로 이동
	sub bl,48	; bl에 0이나 1이 저장되어있으면 아스키코드 0만큼 빼줌
	add al,bl	; eax에 더해줌 ( 0이입력되면 +0 1이입력되어있으면 +1)
	loop L1
L2:
	mov ecx,0
	ret
Checkstring ENDP

Sumn PROC
	cmp eax, 0 ;eax가 0이면 종료
	je S1
	add ecx,eax  
	dec eax
	call Sumn
S1:
	ret 
Sumn ENDP
END main