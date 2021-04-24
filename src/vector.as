stc #1 #0
stc #2 #1
stc #3 #2
stc #4 #3
stc #5 #4
stc #1 #5
stc #2 #6
stc #3 #7
stc #4 #8
stc #5 #9

ldc r0 #0       //counter
ldc r1 #0       //&a[0]
ldc r2 #5       //&b[0]
ldc r3 #10      //&c[0]

add lr pc #0   //lr<-pc+1
ld r4 r1 r0    //r4 <- a[i]
ld r5 r2 r0    //r5 <- b[i]
add r6 r4 r5    // r6 <- r4 + r5
st r6 r3 r0    // c[i] = r6
add r0 r0 #1   // i++
blt lr r0 r2       // &b[0] < i
halt