TITLE Recurrence relation calculation
;20141575 ÀÓÇü¼®

INCLUDE Irvine32.inc

.data
INCLUDE HW3_include.inc

.code
main PROC
	mov EAX,a1		;EAX=a1
	mov EBX,b1		;EBX=b1
	mov EDX,EAX		;EDX=a1
	add EAX,EBX		;EAX=a2(a1+b1)
	add EDX,EAX		;EDX=a2+a1
	sub EDX,EBX		;EDX=a2+a1-b1
	sub EDX,EBX		;EDX=a2+a1-2b1
	sub EDX,EBX		;EDX=a2+a1-3b1
	mov EBX,EDX		;EBX=b2	

	mov EDX,EAX		;EDX=a2
	add EAX,EBX		;EAX=a3(a2+b2)
	add EDX,EAX		;EDX=a3+a2
	sub EDX,EBX		;EDX=a3+a2-b2
	sub EDX,EBX		;EDX=a3+a2-2b2
	sub EDX,EBX		;EDX=a3+a2-3b2
	mov EBX,EDX		;EBX=b3

	mov EDX,EAX		;EDX=a3
	add EAX,EBX		;EAX=a4(a3+b3)
	add EDX,EAX		;EDX=a4+a3
	sub EDX,EBX		;EDX=a4+a3-b3
	sub EDX,EBX		;EDX=a4+a3-2b3
	sub EDX,EBX		;EDX=a4+a3-3b3
	mov EBX,EDX		;EBX=b4

	mov EDX,EAX		;EDX=a4
	add EAX,EBX		;EAX=a5(a4+b4)
	add EDX,EAX		;EDX=a5+a4
	sub EDX,EBX		;EDX=a5+a4-b4
	sub EDX,EBX		;EDX=a5+a4-2b4
	sub EDX,EBX		;EDX=a5+a4-3b4
	mov EBX,EDX		;EBX=b5

	mov EDX,EAX		;EDX=a5
	add EAX,EBX		;EAX=a6(a5+b5)
	add EDX,EAX		;EDX=a6+a5
	sub EDX,EBX		;EDX=a6+a5-b5
	sub EDX,EBX		;EDX=a6+a5-2b5
	sub EDX,EBX		;EDX=a6+a5-3b5
	mov EBX,EDX		;EBX=b6

	mov EDX,EAX		;EDX=a6
	add EAX,EBX		;EAX=a6(a5+b5)
	add EDX,EAX		;EDX=a6+a5
	sub EDX,EBX		;EDX=a6+a5-b5
	sub EDX,EBX		;EDX=a6+a5-2b5
	sub EDX,EBX		;EDX=a6+a5-3b5
	mov EBX,EDX		;EBX=b7

	call DumpRegs	;display registers
	exit
main ENDP
END main