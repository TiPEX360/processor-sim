stc #93 #0
stc #3 #1
stc #102 #2
stc #15 #3
stc #35 #4
stc #75 #5
stc #93 #6
stc #102 #7
stc #80 #8
stc #5 #9
stc #1 #10
stc #7 #11
stc #85 #12
stc #3 #13
stc #63 #14
stc #90 #15
stc #126 #16
stc #83 #17
stc #59 #18
stc #57 #19
stc #14 #20
stc #48 #21
stc #45 #22
stc #84 #23
stc #84 #24
stc #69 #25
stc #30 #26
stc #70 #27
stc #2 #28
stc #41 #29
stc #42 #30
stc #86 #31
stc #35 #32
stc #16 #33
stc #101 #34
stc #61 #35
stc #82 #36
stc #58 #37
stc #35 #38
stc #34 #39
stc #62 #40
stc #28 #41
stc #32 #42
stc #11 #43
stc #30 #44
stc #94 #45
stc #100 #46
stc #20 #47
stc #42 #48
stc #24 #49
stc #68 #50
stc #47 #51
stc #71 #52
stc #112 #53
stc #122 #54
stc #27 #55
stc #54 #56
stc #24 #57
stc #89 #58
stc #55 #59
stc #57 #60
stc #3 #61
stc #14 #62
stc #83 #63
stc #10 #64
stc #106 #65
stc #17 #66
stc #83 #67
stc #36 #68
stc #43 #69
stc #116 #70
stc #89 #71
stc #70 #72
stc #13 #73
stc #99 #74
stc #91 #75
stc #98 #76
stc #63 #77
stc #110 #78
stc #4 #79
stc #86 #80
stc #50 #81
stc #50 #82
stc #29 #83
stc #27 #84
stc #45 #85
stc #48 #86
stc #80 #87
stc #60 #88
stc #9 #89
stc #126 #90
stc #116 #91
stc #3 #92
stc #4 #93
stc #64 #94
stc #12 #95
stc #109 #96
stc #80 #97
stc #94 #98
stc #9 #99

ldc r0 #0 //i
ldc r4 #99 //size - 1

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
blt #103 r1 r4 //branch inner loop
add r0 r0 #1 //i++
blt #102 r0 r4 //branch outer loop
halt