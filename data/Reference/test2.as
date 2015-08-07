MAIN:	mov1 K, LENGTH
	add1 r2,STR
LOOP:	jmp1 END
	prn1 #-5
	sub1 r1, r4
	inc1 K
	mov1 $$,r3
	bne1 LOOP
END:	stop1
STR:	.string "abcdef"
LENGTH:	.data 6,-9,15
K:	.data 2

