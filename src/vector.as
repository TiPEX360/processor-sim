sto #1 #0 #0
sto #2 #0 #1
sto #3 #0 #2
sto #4 #0 #3
sto #5 #0 #4
sto #1 #0 #5
sto #2 #0 #6
sto #3 #0 #7
sto #4 #0 #8
sto #5 #0 #9

ldc r0 #0       //counter
ldc r1 #0       //&a[0]
ldc r2 #5       //&b[0]
ldc r3 #10      //&c[0]

addc lr pc #1   //lr<-pc+1
ldm r4 r1 r0    //r4 <- a[i]
ldm r5 r2 r0    //r5 <- b[i]
add r6 r4 r5    // r6 <- r4 + r5
stm r6 r3 r0    // c[i] = r6
addc r0 r0 #1   // i++
blt lr r0 r2       // &b[0] < i
halt