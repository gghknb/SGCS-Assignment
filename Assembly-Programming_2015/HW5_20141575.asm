TITLE recursive (HW5_20141575.asm)

INCLUDE Irvine32.inc

.data
INCLUDE HW5_include.inc

.code
main PROC
	mov eax,0
	mov ecx,31				 ;bit�� string�� 31�� ���� ������ ecx�� 31�� ����
	mov esi,OFFSET stringEx1 ;string�� �ּҰ��� ����
	call Checkstring		 ; string���� 1�� ������ check���ִ� checkstring�Լ� ȣ��
	call Sumn				 ; 1���� n���� �����ִ� �Լ� ȣ��
	mov eax,ecx
	call WriteDec
	call crlf
	exit
main ENDP

Checkstring PROC
L1:
	mov ebx,[esi]  ;ebx�� string�� ù��° ���ڰ� ����
	inc esi		   
	cmp bl,48	;'0'�� ��
	jb L2		;'0'���� ������ L2�� �̵�
	cmp bl,49	;'1'����
	ja L2		;'1'���� ũ�� L2�� �̵�
	sub bl,48	; bl�� 0�̳� 1�� ����Ǿ������� �ƽ�Ű�ڵ� 0��ŭ ����
	add al,bl	; eax�� ������ ( 0���ԷµǸ� +0 1���ԷµǾ������� +1)
	loop L1
L2:
	mov ecx,0
	ret
Checkstring ENDP

Sumn PROC
	cmp eax, 0 ;eax�� 0�̸� ����
	je S1
	add ecx,eax  
	dec eax
	call Sumn
S1:
	ret 
Sumn ENDP
END main