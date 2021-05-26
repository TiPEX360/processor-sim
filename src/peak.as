stc #63 #0
stc #127 #1
stc #130 #2
stc #88 #3
stc #21 #4
stc #24 #5
stc #35 #6
stc #122 #7
stc #73 #8
stc #54 #9
stc #55 #10
stc #120 #11
stc #12 #12
stc #118 #13
stc #62 #14
stc #19 #15
stc #119 #16
stc #82 #17
stc #81 #18
stc #118 #19
stc #15 #20
stc #2 #21
stc #69 #22
stc #47 #23
stc #76 #24


ldc r1 #0
ldc r2 #25

ldc r16 #0

sub r3 r2 r1 //peakfind 28
sub r4 r3 #0
bnz #33 r4 //lentwo
add r16 r16 #0
b #56 //print

sub r4 r3 #2 //lentwo 33
bnz #40 r4 //center
ld r4 r1 #0
ld r5 r1 #1
blt #56 r5 r4 //print
add r16 r16 #1
b #56 //print

div r3 r3 #2 //center 40
sub r4 r3 #1 //mistake? nah, just gets left of middle
ld r4 r1 r4
ld r5 r1 r3
add r6 r3 #1
ld r6 r1 r6

blt #50 r5 r4 //checkleft 
blt #52 r5 r6 //checkright 
add r16 r16 r3
b #56 //print


sub r2 r2 r3 //checkleft 50
b #28 //peekfind


add r3 r3 #1 //checkright 52
add r16 r16 r3
add r1 r1 r3
b #28 //peekfind


//print 56
st r16 r20 #100 
halt