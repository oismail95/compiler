LOAD 7
STORE id1
LOAD 2
STORE T1
LOAD id1
SUB T1
BRZNEG L1
L2: NOOP
LOAD 7
STORE T2
LOAD id1
SUB T2
BRNEG L3
BRPOS L3
LOAD 31
STORE id1
BR L2
L3: NOOP
L1: NOOP
LOAD id1
STORE T3
WRITE T3
STOP
id2 0
id1 0
T1 0
T2 0
T3 0