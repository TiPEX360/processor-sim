st #9 #0
st #5 #1
st #3 #2
st #8 #3
st #9 #4
st #32 #5
st #22 #6
st #43 #7
st #3 #8
st #60 #9

ldc r0 #0 //i
ldc r4 #9 //size - 1

add r10 pc #1 //outer loop start
ldc r1 #0 //j

add r11 pc #1 //inner loop start
ld r2 r1 #0 //a
ld r3 r1 #1 //b

jlt #3 r2 r3 //a < b?
st r2 r1 #1 //swap
st r3 r1 #0


add r1 r1 #1 //j++
blt r11 r1 r4 //branch inner loop
add r0 r0 #1 //i++
blt r10 r0 r4 //branch outer loop
halt