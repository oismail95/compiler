LOAD 16
STORE id1
LOAD 33
STORE id3
LOAD 14
STORE T1
LOAD id1
SUB T1
BRZNEG L1
LOAD id3
STORE T2
WRITE T2
L1: NOOP
STOP
id3 0
id2 0
id1 0
T1 0
T2 0