stc #52 #0
stc #18 #1

ldc r5 #0 //memory start

ld r0 r5 #0
ld r1 r5 #1


add lr pc #0 //start euclid
jnz #2 r1 // r1 == 0?
st r0 r5 #2 // store answer
halt
div r3 r0 r1    // else
mul r3 r3 r1
sub r3 r0 r3    // r3 = remainder
add r0 r1 #0   // swap parameters
add r1 r3 #0   
b lr            // loop
