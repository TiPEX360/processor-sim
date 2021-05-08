stc #52 #0
stc #18 #1

ldc r5 #0 //memory start

//nop
//nop
ld r0 r5 #0
ld r1 r5 #1

add lr pc #0 //start euclid
//nop
jnz #2 r1 // r1 == 0? ////RAW HAZARD: r1 decoded before ln:7 is written back to registers RAW
st r0 r5 #2 // store answer
halt
div r3 r0 r1    // else
//nop
//nop
mul r3 r3 r1
//nop
//nop
sub r3 r0 r3    // r3 = remainder
add r0 r1 #0   // swap parameters
//nop
add r1 r3 #0   
b #6            // loop
halt