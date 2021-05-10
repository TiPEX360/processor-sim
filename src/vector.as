stc #99 #0
stc #106 #1
stc #96 #2
stc #91 #3
stc #13 #4
stc #95 #5
stc #41 #6
stc #90 #7
stc #34 #8
stc #78 #9
stc #126 #10
stc #117 #11
stc #76 #12
stc #118 #13
stc #15 #14
stc #4 #15
stc #62 #16
stc #94 #17
stc #39 #18
stc #75 #19
stc #31 #20
stc #48 #21
stc #112 #22
stc #45 #23
stc #39 #24
stc #20 #25
stc #97 #26
stc #31 #27
stc #105 #28
stc #89 #29
stc #92 #30
stc #75 #31
stc #66 #32
stc #59 #33
stc #38 #34
stc #79 #35
stc #25 #36
stc #78 #37
stc #40 #38
stc #58 #39
stc #28 #40
stc #37 #41
stc #46 #42
stc #103 #43
stc #27 #44
stc #60 #45
stc #106 #46
stc #88 #47
stc #25 #48
stc #17 #49
stc #34 #50
stc #56 #51
stc #64 #52
stc #17 #53
stc #100 #54
stc #102 #55
stc #37 #56
stc #68 #57
stc #4 #58
stc #13 #59
stc #28 #60
stc #95 #61
stc #87 #62
stc #94 #63
stc #25 #64
stc #124 #65
stc #44 #66
stc #50 #67
stc #74 #68
stc #83 #69
stc #107 #70
stc #101 #71
stc #119 #72
stc #25 #73
stc #75 #74
stc #17 #75
stc #84 #76
stc #53 #77
stc #104 #78
stc #109 #79
stc #69 #80
stc #10 #81
stc #36 #82
stc #4 #83
stc #26 #84
stc #7 #85
stc #105 #86
stc #62 #87
stc #74 #88
stc #108 #89
stc #74 #90
stc #102 #91
stc #74 #92
stc #33 #93
stc #67 #94
stc #98 #95
stc #28 #96
stc #110 #97
stc #19 #98
stc #101 #99

ldc r0 #0       //counter
ldc r1 #0       //&a[0]
ldc r2 #50       //&b[0]
ldc r3 #100      //&c[0]

//add lr pc #0   //lr<-pc+1
ld r4 r1 r0    //r4 <- a[i]
ld r5 r2 r0    //r5 <- b[i]
add r6 r4 r5    // r6 <- r4 + r5
st r6 r3 r0    // c[i] = r6
add r0 r0 #1   // i++
//blt lr r0 r2       // &b[0] < i
blt #104 r0 r2       // &b[0] < i
halt