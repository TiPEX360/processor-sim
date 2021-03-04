sto #52 #0 #0
sto #18 #0 #1

ldc r5 #0 //memory start

ldmc r0 r5 #0
ldmc r1 r5 #1

addc lr pc #1 //start euclid

jnz #3 r1 //!!!!!
stmc r0 r5 #2 // r1 == 0
halt
div r3 r0 r1      //r1 != 0
mul r3 r3 r1
sub r3 r0 r3    //r3 = remainder
addc r0 r1 #0   //r0 <- r1
addc r1 r3 #0   //r1 <- r3 (remainder)
b lr           
