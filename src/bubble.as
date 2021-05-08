stc #7 #0
stc #5 #1
stc #3 #2
stc #8 #3
stc #9 #4
stc #32 #5
stc #22 #6
stc #43 #7
stc #3 #8
stc #25 #9

ldc r0 #0 //i
ldc r4 #9 //size - 1

//add r10 pc #0 //outer loop start
ldc r1 #0 //j

//add r11 pc #0 //inner loop start
//nop
ld r2 r1 #0 //a
ld r3 r1 #1 //b

//nop
//nop
jlt #2 r2 r3 //a < b? //check #15->#18 jumped to
st r2 r1 #1 //swap
st r3 r1 #0


add r1 r1 #1 //j++
blt #13 r1 r4 //branch inner loop
add r0 r0 #1 //i++
blt #12 r0 r4 //branch outer loop
halt