sto #9 #0 #0
sto #5 #0 #1
sto #3 #0 #2
sto #8 #0 #3
sto #9 #0 #4
sto #32 #0 #5
sto #22 #0 #6
sto #43 #0 #7
sto #3 #0 #8
sto #60 #0 #9

ldc r0 #0 //i
ldc r4 #9 //size - 1

addc r10 pc #1 //outer loop start
ldc r1 #0 //j

addc r11 pc #1 //inner loop start
ldmc r2 r1 #0 //a
ldmc r3 r1 #1 //b

jlt #3 r2 r3 //a < b?
stmc r2 r1 #1 //swap
stmc r3 r1 #0


addc r1 r1 #1 //j++
blt r11 r1 r4
addc r0 r0 #1 //i++
blt r10 r0 r4
halt