MAIN: mov1 	K, 	W
	add2 	$$,	STR
	mov1 	$$,	r4
	jmp2	$$

	clr1 	r3
	clr2	r7
	red1		$$
	
END: stop1	
	
.entry MAIN	
.extern W
K: .data 2 
STR: .string "abcdef=123"