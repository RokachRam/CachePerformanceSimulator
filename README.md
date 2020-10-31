# CachePerformanceSimulator

Cache simulator to evaluate Performance and miss rates implemented in C.

This Simulator will calculate the miss rate of 3 different cache assosiativities.

1 - Way  (1)
2 - Way  (2)
Fully assosiative  (F)

There are 3 text files which contains Byte's access adress in memory.

The simulator will also consider the Number of bytes and the block size.

For example: cache.exe 1024 F 32 trace1.txt
Will execute the simulator with the parameters: 
Number of bytes: 1024
Assosiativity: Fully assosiative
Block size: 32

The code will provide the miss rate which is the: number of misses / number of lines.

