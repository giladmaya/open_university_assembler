MAIN:	mov1 K, LENGTH
	add1 r2,STR
LOOP:	jmp1 END
	prn1 #-5
	sub1 #1, r1
	inc1 r0
	mov1 $$,r3
	bne1 LOOP
END:	stop1
STR:	.string "abcdef"
LENGTH:	.data 6,-9,15
K:	.data 2

