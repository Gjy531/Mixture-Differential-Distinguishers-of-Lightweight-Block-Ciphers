This repository contains supplementary materials of the paper
# Security Evaluation of Lightweight Block Ciphers against Mixture Differential Cryptanalysis


The structure of the folder is as follows
- [SKINNY](#SKINNY)
- [MANTIS](#MANTIS)
- [CRAFT](#CRAFT)
- [Midori](#Midori)
- [LBlock](#LBlock)
- [TWINE](#TWINE)
- [WARP](#WARP)

All the experiments are executed on a laptop with 8 logic processors.  

[Gurobi]: https://www.gurobi.com/

[SKINNY] folder contains the search codes for generating the mixture differential distinguishers and the verification codes to verify the distinguishers we found. 

To generate the distinguishers , preinstallion is [Gurobi]. This work is done by version 10.0.

To get the results, run

```sh
g++ Skinny.cpp basic.cpp -I/$GUROBI_HOME/include -L/$GUROBI_HOME/lib -lgurobi_c++ -lgurobi100 -lm -o Skinny.out

nohup ./Skinny.out
```

To run the verification programm, MPI package need to be installed and change the library path and include path in Makefile according to your installation. Run  `mingw32-make` (in Windows) to compile the C++ codes.

To test 7-round by running on 8 processors, run

```sh
mpiexec -n 8 SKINNY_Quadruple_MPI_RNG 7
```
You get a summary of number of quaries and number of right quadruples on each process, total queries, number of total right quadruples and the probability. We also reserved codes (commented in current version) for comparison with full round SKINNY as random permutation. 
```
[+] Proc 0
[+] PRNG initialized to 0xEC260DF0

 Quaries on each process = 2^(16.000000)
 0 proc:     Right = 2^(4.857981)
 0 proc: randRight = 2^(0.000000)
 1 proc:     Right = 2^(4.584963)
 1 proc: randRight = 2^(-inf)
 2 proc:     Right = 2^(5.000000)
 2 proc: randRight = 2^(1.584963)
 3 proc:     Right = 2^(4.954196)
 3 proc: randRight = 2^(-inf)
 4 proc:     Right = 2^(4.754888)
 4 proc: randRight = 2^(0.000000)
 5 proc:     Right = 2^(5.169925)
 5 proc: randRight = 2^(0.000000)
 6 proc:     Right = 2^(4.523562)
 6 proc: randRight = 2^(-inf)
 7 proc:     Right = 2^(4.954196)
 7 proc: randRight = 2^(0.000000)

                              Total queries = 2^(19.000000)
Skinny:     Number of total right quadruples = 2^(7.864186)
           Probability of right quadruples  = 2^(-11.1358)
 RandPerm: Number of total right quadruples = 2^(2.807355)
            Probability of right quadruples = 2^(-16.1926)
 time on clock(): 0.327

[+] Time : 0.319375
[+] Skinny:      Number of right quadruples = 2^(4.857981)
[+]           Probability of right quadruples = 2^(-11.1420)

[+] RandPerm:      Number of right quadruples = 2^(0.000000)
[+]           Probability of right quadruples = 2^(-16.0000)


[+] Proc 2
[+] PRNG initialized to 0xEC260DF2
[+] Time : 0.325180
[+] Skinny:      Number of right quadruples = 2^(5.000000)
[+]           Probability of right quadruples = 2^(-11.0000)

[+] RandPerm:      Number of right quadruples = 2^(1.584963)
[+]           Probability of right quadruples = 2^(-14.4150)


[+] Proc 7
[+] PRNG initialized to 0xEC260DF7
[+] Time : 0.319456
[+] Skinny:      Number of right quadruples = 2^(4.954196)
[+]           Probability of right quadruples = 2^(-11.0458)

[+] RandPerm:      Number of right quadruples = 2^(0.000000)
[+]           Probability of right quadruples = 2^(-16.0000)


[+] Proc 1
[+] PRNG initialized to 0xEC260DF1
[+] Time : 0.315805
[+] Skinny:      Number of right quadruples = 2^(4.584963)
[+]           Probability of right quadruples = 2^(-11.4150)

[+] RandPerm:      Number of right quadruples = 2^(-inf)
[+]           Probability of right quadruples = 2^(-inf)


[+] Proc 4
[+] PRNG initialized to 0xEC260DF4
[+] Time : 0.316856
[+] Skinny:      Number of right quadruples = 2^(4.754888)
[+]           Probability of right quadruples = 2^(-11.2451)

[+] RandPerm:      Number of right quadruples = 2^(0.000000)
[+]           Probability of right quadruples = 2^(-16.0000)


[+] Proc 6
[+] PRNG initialized to 0xEC260DF6
[+] Time : 0.322052
[+] Skinny:      Number of right quadruples = 2^(4.523562)
[+]           Probability of right quadruples = 2^(-11.4764)

[+] RandPerm:      Number of right quadruples = 2^(-inf)
[+]           Probability of right quadruples = 2^(-inf)


[+] Proc 5
[+] PRNG initialized to 0xEC260DF5
[+] Time : 0.315282
[+] Skinny:      Number of right quadruples = 2^(5.169925)
[+]           Probability of right quadruples = 2^(-10.8301)

[+] RandPerm:      Number of right quadruples = 2^(0.000000)
[+]           Probability of right quadruples = 2^(-16.0000)


[+] Proc 3
[+] PRNG initialized to 0xEC260DF3
[+] Time : 0.320933
[+] Skinny:      Number of right quadruples = 2^(4.954196)
[+]           Probability of right quadruples = 2^(-11.0458)

[+] RandPerm:      Number of right quadruples = 2^(-inf)
[+]           Probability of right quadruples = 2^(-inf)
```



[MANTIS] This folder contains two subfolders: search and verification. The search folder contains source codes to search for the mixture differential distinguishers covering 4-  6- and 8- round of Mantis. The codes are launched on Linux OS. To run our codes, preinstallation is Gurobi for solving constraint programming problems. To get the 15 mixture trails for 5-round Mantis, run

+ `g++ Mantis1.cpp basic.cpp -I/$GUROBI_HOME/include -L/$GUROBI_HOME/lib -lgurobi_c++ -lgurobi100 -lm -o Mantis1.out`
+ `nohup ./Mantis1.out`

The verification folder contains the verification codes to verify the quadruple distinguishers we found. To run the programm, MPI package need to be installed and change the library path and include path in Makefile according to your installation path.  Run `mingw32-make` (in Windows) to compile the C++ codes. 

To test 6-round in algorithm with 14-round as parameter in random permutaion by running on 8 processors, run

```sh
mpiexec -n 8 MANTIS_Quadruple_MPI_RNG 6 14
```

You get a summary of number of quaries and number of right quadruples on each process, total queries, number of total right quadruples and the probability. We also reserved codes (commented in current version) for comparison with full round MANTIS as random permutation. 

Results on 6-round with 14-round as parameter in random permutaion：

```
[+] Proc 0
[+] PRNG initialized to 0xF113BEB6

 Quaries on each process = 2^(16.000000)
 0 proc:     Right = 2^(8.400879)
 0 proc: randRight = 2^(0.000000)
 1 proc:     Right = 2^(8.375039)
 1 proc: randRight = 2^(0.000000)
 2 proc:     Right = 2^(8.164907)
 2 proc: randRight = 2^(0.000000)
 3 proc:     Right = 2^(8.071462)
 3 proc: randRight = 2^(0.000000)
 4 proc:     Right = 2^(8.179909)
 4 proc: randRight = 2^(1.000000)
 5 proc:     Right = 2^(8.087463)
 5 proc: randRight = 2^(0.000000)
 6 proc:     Right = 2^(8.303781)
 6 proc: randRight = 2^(0.000000)
 7 proc:     Right = 2^(8.022368)
 7 proc: randRight = 2^(1.000000)

                              Total queries = 2^(19.000000)
MANTIS:     Number of total right quadruples = 2^(11.207014)
           Probability of right quadruples  = 2^(-7.7930)
 RandPerm: Number of total right quadruples = 2^(3.321928)
            Probability of right quadruples = 2^(-15.6781)
 time on clock(): 0.25


[+] Proc 2
[+] PRNG initialized to 0xF113BEB8
[+] Time : 0.239844
[+] MANTIS:         Number of right quadruples = 2^(8.164907)
[+]           Probability of right quadruples = 2^(-7.8351)

[+] RandPerm:      Number of right quadruples = 2^(0.000000)
[+]           Probability of right quadruples = 2^(-16.0000)


[+] Proc 3
[+] PRNG initialized to 0xF113BEB9
[+] Time : 0.238484
[+] MANTIS:         Number of right quadruples = 2^(8.071462)
[+]           Probability of right quadruples = 2^(-7.9285)

[+] RandPerm:      Number of right quadruples = 2^(0.000000)
[+]           Probability of right quadruples = 2^(-16.0000)


[+] Proc 6
[+] PRNG initialized to 0xF113BEBC
[+] Time : 0.238071
[+] MANTIS:         Number of right quadruples = 2^(8.303781)
[+]           Probability of right quadruples = 2^(-7.6962)

[+] RandPerm:      Number of right quadruples = 2^(0.000000)
[+]           Probability of right quadruples = 2^(-16.0000)


[+] Proc 7
[+] PRNG initialized to 0xF113BEBD
[+] Time : 0.237936
[+] MANTIS:         Number of right quadruples = 2^(8.022368)
[+]           Probability of right quadruples = 2^(-7.9776)

[+] RandPerm:      Number of right quadruples = 2^(1.000000)
[+]           Probability of right quadruples = 2^(-15.0000)

[+] Time : 0.238200
[+] MANTIS:         Number of right quadruples = 2^(8.400879)
[+]           Probability of right quadruples = 2^(-7.5991)

[+] RandPerm:      Number of right quadruples = 2^(0.000000)
[+]           Probability of right quadruples = 2^(-16.0000)


[+] Proc 5
[+] PRNG initialized to 0xF113BEBB
[+] Time : 0.240682
[+] MANTIS:         Number of right quadruples = 2^(8.087463)
[+]           Probability of right quadruples = 2^(-7.9125)

[+] RandPerm:      Number of right quadruples = 2^(0.000000)
[+]           Probability of right quadruples = 2^(-16.0000)


[+] Proc 1
[+] PRNG initialized to 0xF113BEB7
[+] Time : 0.241979
[+] MANTIS:         Number of right quadruples = 2^(8.375039)
[+]           Probability of right quadruples = 2^(-7.6250)

[+] RandPerm:      Number of right quadruples = 2^(0.000000)
[+]           Probability of right quadruples = 2^(-16.0000)


[+] Proc 4
[+] PRNG initialized to 0xF113BEBA
[+] Time : 0.240845
[+] MANTIS:         Number of right quadruples = 2^(8.179909)
[+]           Probability of right quadruples = 2^(-7.8201)

[+] RandPerm:      Number of right quadruples = 2^(1.000000)
[+]           Probability of right quadruples = 2^(-15.0000)
```



[CRAFT] folder contains the search codes for generating the mixture differential distinguishers and the verification codes to verify the distinguishers we found. 

To generate the distinguishers , preinstallion is [Gurobi]. This work is done by version 10.0.

To get the results, run

```sh
g++ CRAFT.cpp basic.cpp -I/$GUROBI_HOME/include -L/$GUROBI_HOME/lib -lgurobi_c++ -lgurobi100 -lm -o CRAFT.out
m
nohup ./CRAFT.out
```

To run the verification programm, MPI package need to be installed and change the library path and include path in Makefile according to your installation. Run  `mingw32-make`  (in Windows) to compile the C++ codes.

To test 6-round by running on 8 processors, run

```sh
mpiexec -n 8 CRAFT_Quadruple_MPI_RNG 6
```

You get a summary of number of quaries and number of right quadruples on each process, total queries, number of total right quadruples and the probability. We also reserved codes (commented in current version) for comparison with full round CRAFT as random permutation. 

```
[+] Proc 0
[+] PRNG initialized to 0xD19EC404

 Quaries on each process = 2^(8.000000)
 0 proc:     Right = 2^(8.000000)
 1 proc:     Right = 2^(8.000000)
 2 proc:     Right = 2^(8.000000)
 3 proc:     Right = 2^(8.000000)
 4 proc:     Right = 2^(8.000000)
 5 proc:     Right = 2^(8.000000)
 6 proc:     Right = 2^(8.000000)
 7 proc:     Right = 2^(8.000000)

                              Total queries = 2^(11.000000)
CRAFT:     Number of total right quadruples = 2^(11.000000)
           Probability of right quadruples  = 2^(-0.0000)
 time on clock(): 0.002


[+] Proc 6
[+] PRNG initialized to 0xD19EC40A
[+] Time : 0.000394
[+] CRAFT:         Number of right quadruples = 2^(8.000000)
[+]           Probability of right quadruples = 2^(-0.0000)


[+] Proc 4
[+] PRNG initialized to 0xD19EC408
[+] Time : 0.000409
[+] CRAFT:         Number of right quadruples = 2^(8.000000)
[+]           Probability of right quadruples = 2^(-0.0000)


[+] Proc 2
[+] PRNG initialized to 0xD19EC406
[+] Time : 0.000359
[+] CRAFT:         Number of right quadruples = 2^(8.000000)
[+]           Probability of right quadruples = 2^(-0.0000)


[+] Proc 7
[+] PRNG initialized to 0xD19EC40B
[+] Time : 0.000415
[+] CRAFT:         Number of right quadruples = 2^(8.000000)
[+]           Probability of right quadruples = 2^(-0.0000)

[+] Time : 0.000403
[+] CRAFT:         Number of right quadruples = 2^(8.000000)
[+]           Probability of right quadruples = 2^(-0.0000)


[+] Proc 5
[+] PRNG initialized to 0xD19EC409
[+] Time : 0.000405
[+] CRAFT:         Number of right quadruples = 2^(8.000000)
[+]           Probability of right quadruples = 2^(-0.0000)


[+] Proc 1
[+] PRNG initialized to 0xD19EC405
[+] Time : 0.000408
[+] CRAFT:         Number of right quadruples = 2^(8.000000)
[+]           Probability of right quadruples = 2^(-0.0000)


[+] Proc 3
[+] PRNG initialized to 0xD19EC407
[+] Time : 0.000401
[+] CRAFT:         Number of right quadruples = 2^(8.000000)
[+]           Probability of right quadruples = 2^(-0.0000)

```

Results on 7-round：

```
[+] Proc 0
[+] PRNG initialized to 0xD19EE056

 Quaries on each process = 2^(8.000000)
 0 proc:     Right = 2^(8.000000)
 1 proc:     Right = 2^(8.000000)
 2 proc:     Right = 2^(8.000000)
 3 proc:     Right = 2^(8.000000)
 4 proc:     Right = 2^(8.000000)
 5 proc:     Right = 2^(8.000000)
 6 proc:     Right = 2^(8.000000)
 7 proc:     Right = 2^(8.000000)

                              Total queries = 2^(11.000000)
CRAFT:     Number of total right quadruples = 2^(11.000000)
           Probability of right quadruples  = 2^(-0.0000)
 time on clock(): 0.003


[+] Proc 1
[+] PRNG initialized to 0xD19EE057
[+] Time : 0.000440
[+] CRAFT:         Number of right quadruples = 2^(8.000000)
[+]           Probability of right quadruples = 2^(-0.0000)

[+] Time : 0.000414
[+] CRAFT:         Number of right quadruples = 2^(8.000000)
[+]           Probability of right quadruples = 2^(-0.0000)


[+] Proc 7
[+] PRNG initialized to 0xD19EE05D
[+] Time : 0.000439
[+] CRAFT:         Number of right quadruples = 2^(8.000000)
[+]           Probability of right quadruples = 2^(-0.0000)


[+] Proc 2
[+] PRNG initialized to 0xD19EE058
[+] Time : 0.000435
[+] CRAFT:         Number of right quadruples = 2^(8.000000)
[+]           Probability of right quadruples = 2^(-0.0000)


[+] Proc 6
[+] PRNG initialized to 0xD19EE05C
[+] Time : 0.000433
[+] CRAFT:         Number of right quadruples = 2^(8.000000)
[+]           Probability of right quadruples = 2^(-0.0000)


[+] Proc 3
[+] PRNG initialized to 0xD19EE059
[+] Time : 0.000433
[+] CRAFT:         Number of right quadruples = 2^(8.000000)
[+]           Probability of right quadruples = 2^(-0.0000)


[+] Proc 5
[+] PRNG initialized to 0xD19EE05B
[+] Time : 0.000424
[+] CRAFT:         Number of right quadruples = 2^(8.000000)
[+]           Probability of right quadruples = 2^(-0.0000)


[+] Proc 4
[+] PRNG initialized to 0xD19EE05A
[+] Time : 0.000274
[+] CRAFT:         Number of right quadruples = 2^(8.000000)
[+]           Probability of right quadruples = 2^(-0.0000)
```

Results on 8-round:

```
[+] Proc 0
[+] PRNG initialized to 0xD19EEA06

 Quaries on each process = 2^(16.000000)
 0 proc:     Right = 2^(13.506308)
 1 proc:     Right = 2^(13.512123)
 2 proc:     Right = 2^(13.535883)
 3 proc:     Right = 2^(13.534303)
 4 proc:     Right = 2^(13.511629)
 5 proc:     Right = 2^(13.535761)
 6 proc:     Right = 2^(13.505067)
 7 proc:     Right = 2^(13.521478)

                              Total queries = 2^(19.000000)
CRAFT:     Number of total right quadruples = 2^(16.520373)
           Probability of right quadruples  = 2^(-2.4796)
 time on clock(): 0.103

[+] Time : 0.101380
[+] CRAFT:         Number of right quadruples = 2^(13.506308)
[+]           Probability of right quadruples = 2^(-2.4937)


[+] Proc 1
[+] PRNG initialized to 0xD19EEA07
[+] Time : 0.102427
[+] CRAFT:         Number of right quadruples = 2^(13.512123)
[+]           Probability of right quadruples = 2^(-2.4879)


[+] Proc 6
[+] PRNG initialized to 0xD19EEA0C
[+] Time : 0.098035
[+] CRAFT:         Number of right quadruples = 2^(13.505067)
[+]           Probability of right quadruples = 2^(-2.4949)


[+] Proc 7
[+] PRNG initialized to 0xD19EEA0D
[+] Time : 0.100957
[+] CRAFT:         Number of right quadruples = 2^(13.521478)
[+]           Probability of right quadruples = 2^(-2.4785)


[+] Proc 5
[+] PRNG initialized to 0xD19EEA0B
[+] Time : 0.098529
[+] CRAFT:         Number of right quadruples = 2^(13.535761)
[+]           Probability of right quadruples = 2^(-2.4642)


[+] Proc 2
[+] PRNG initialized to 0xD19EEA08
[+] Time : 0.097905
[+] CRAFT:         Number of right quadruples = 2^(13.535883)
[+]           Probability of right quadruples = 2^(-2.4641)


[+] Proc 3
[+] PRNG initialized to 0xD19EEA09
[+] Time : 0.099072
[+] CRAFT:         Number of right quadruples = 2^(13.534303)
[+]           Probability of right quadruples = 2^(-2.4657)


[+] Proc 4
[+] PRNG initialized to 0xD19EEA0A
[+] Time : 0.100430
[+] CRAFT:         Number of right quadruples = 2^(13.511629)
[+]           Probability of right quadruples = 2^(-2.4884)
```

Results on 9-round:

```
[+] Proc 0
[+] PRNG initialized to 0xD1B60786

 Quaries on each process = 2^(28.000000)
 0 proc:     Right = 2^(-inf)
 1 proc:     Right = 2^(1.000000)
 2 proc:     Right = 2^(1.000000)
 3 proc:     Right = 2^(-inf)
 4 proc:     Right = 2^(1.000000)
 5 proc:     Right = 2^(0.000000)
 6 proc:     Right = 2^(1.000000)
 7 proc:     Right = 2^(-inf)

                              Total queries = 2^(31.000000)
CRAFT:     Number of total right quadruples = 2^(3.169925)
           Probability of right quadruples  = 2^(-27.8301)
 time on clock(): 483.431

[+] Time : 482.615769
[+] CRAFT:         Number of right quadruples = 2^(-inf)
[+]           Probability of right quadruples = 2^(-inf)


[+] Proc 1
[+] PRNG initialized to 0xD1B60787
[+] Time : 480.954765
[+] CRAFT:         Number of right quadruples = 2^(1.000000)
[+]           Probability of right quadruples = 2^(-27.0000)


[+] Proc 3
[+] PRNG initialized to 0xD1B60789
[+] Time : 482.016472
[+] CRAFT:         Number of right quadruples = 2^(-inf)
[+]           Probability of right quadruples = 2^(-inf)


[+] Proc 6
[+] PRNG initialized to 0xD1B6078C
[+] Time : 483.429180
[+] CRAFT:         Number of right quadruples = 2^(1.000000)
[+]           Probability of right quadruples = 2^(-27.0000)


[+] Proc 7
[+] PRNG initialized to 0xD1B6078D
[+] Time : 481.949031
[+] CRAFT:         Number of right quadruples = 2^(-inf)
[+]           Probability of right quadruples = 2^(-inf)


[+] Proc 2
[+] PRNG initialized to 0xD1B60788
[+] Time : 480.121228
[+] CRAFT:         Number of right quadruples = 2^(1.000000)
[+]           Probability of right quadruples = 2^(-27.0000)


[+] Proc 5
[+] PRNG initialized to 0xD1B6078B
[+] Time : 481.509744
[+] CRAFT:         Number of right quadruples = 2^(0.000000)
[+]           Probability of right quadruples = 2^(-28.0000)


[+] Proc 4
[+] PRNG initialized to 0xD1B6078A
[+] Time : 483.342382
[+] CRAFT:         Number of right quadruples = 2^(1.000000)
[+]           Probability of right quadruples = 2^(-27.0000)
```

Results on 10-round:

```
[+] Proc 0
[+] PRNG initialized to 0xD1A66394

 Quaries on each process = 2^(32.000000)
 0 proc:     Right = 2^(-inf)
 1 proc:     Right = 2^(-inf)
 2 proc:     Right = 2^(0.000000)
 3 proc:     Right = 2^(-inf)
 4 proc:     Right = 2^(-inf)
 5 proc:     Right = 2^(0.000000)
 6 proc:     Right = 2^(-inf)
 7 proc:     Right = 2^(-inf)

                              Total queries = 2^(35.000000)
CRAFT:     Number of total right quadruples = 2^(1.000000)
           Probability of right quadruples  = 2^(-34.0000)
 time on clock(): 12063.5

[+] Time : 12053.677600
[+] CRAFT:         Number of right quadruples = 2^(-inf)
[+]           Probability of right quadruples = 2^(-inf)


[+] Proc 1
[+] PRNG initialized to 0xD1A66395
[+] Time : 12048.608903
[+] CRAFT:         Number of right quadruples = 2^(-inf)
[+]           Probability of right quadruples = 2^(-inf)


[+] Proc 7
[+] PRNG initialized to 0xD1A6639B
[+] Time : 12057.809894
[+] CRAFT:         Number of right quadruples = 2^(-inf)
[+]           Probability of right quadruples = 2^(-inf)


[+] Proc 2
[+] PRNG initialized to 0xD1A66396
[+] Time : 12048.206285
[+] CRAFT:         Number of right quadruples = 2^(0.000000)
[+]           Probability of right quadruples = 2^(-32.0000)


[+] Proc 6
[+] PRNG initialized to 0xD1A6639A
[+] Time : 12047.656034
[+] CRAFT:         Number of right quadruples = 2^(-inf)
[+]           Probability of right quadruples = 2^(-inf)


[+] Proc 3
[+] PRNG initialized to 0xD1A66397
[+] Time : 12062.896487
[+] CRAFT:         Number of right quadruples = 2^(-inf)
[+]           Probability of right quadruples = 2^(-inf)


[+] Proc 5
[+] PRNG initialized to 0xD1A66399
[+] Time : 12039.559938
[+] CRAFT:         Number of right quadruples = 2^(0.000000)
[+]           Probability of right quadruples = 2^(-32.0000)


[+] Proc 4
[+] PRNG initialized to 0xD1A66398
[+] Time : 12063.532009
[+] CRAFT:         Number of right quadruples = 2^(-inf)
[+]           Probability of right quadruples = 2^(-inf)
```



[LBlock] folder contains the search codes for generating the mixture differential distinguishers and the verification codes to verify the distinguishers we found. 

To generate the distinguishers , preinstallion is [Gurobi]. This work is done by version 10.0.

To get the results, run

```sh
g++ LBlock.cpp basic.cpp -I/$GUROBI_HOME/include -L/$GUROBI_HOME/lib -lgurobi_c++ -lgurobi100 -lm -o LBlock.out

nohup ./LBlock.out
```

To run the verification programm, MPI package need to be installed and change the library path and include path in Makefile according to your installation. Run  `mingw32-make`  (in Windows) to compile the C++ codes.

To test 9-round by running on 8 processors, run

```sh
mpiexec -n 8 LBlock_Quadruple_MPI_RNG 9
```
You get a summary of number of quaries and number of right quadruples on each process, total queries, number of total right quadruples and the probability. We also reserved codes (commented in current version) for comparison with full round LBlock as random permutation. 
```
[+] Proc 0
[+] PRNG initialized to 0xF25ABFCC

 Quaries on each process = 2^(16.000000)
 0 proc:     Right = 2^(8.082149)
 0 proc: randRight = 2^(0.000000)
 1 proc:     Right = 2^(8.144658)
 1 proc: randRight = 2^(1.584963)
 2 proc:     Right = 2^(8.169925)
 2 proc: randRight = 2^(2.321928)
 3 proc:     Right = 2^(8.066089)
 3 proc: randRight = 2^(1.584963)
 4 proc:     Right = 2^(8.174926)
 4 proc: randRight = 2^(1.584963)
 5 proc:     Right = 2^(8.016808)
 5 proc: randRight = 2^(-inf)
 6 proc:     Right = 2^(7.936638)
 6 proc: randRight = 2^(-inf)
 7 proc:     Right = 2^(8.092757)
 7 proc: randRight = 2^(-inf)

                              Total queries = 2^(19.000000)
LBlock:    Number of total right quadruples = 2^(11.087463)
           Probability of right quadruples  = 2^(-7.9125)
 RandPerm: Number of total right quadruples = 2^(3.906891)
            Probability of right quadruples = 2^(-15.0931)
 time on clock(): 1.105

[+] Time : 1.084345
[+] LBlock:        Number of right quadruples = 2^(8.082149)
[+]           Probability of right quadruples = 2^(-7.9179)

[+] RandPerm:      Number of right quadruples = 2^(0.000000)
[+]           Probability of right quadruples = 2^(-16.0000)


[+] Proc 6
[+] PRNG initialized to 0xF25ABFD2
[+] Time : 1.102303
[+] LBlock:        Number of right quadruples = 2^(7.936638)
[+]           Probability of right quadruples = 2^(-8.0634)

[+] RandPerm:      Number of right quadruples = 2^(-inf)
[+]           Probability of right quadruples = 2^(-inf)


[+] Proc 1
[+] PRNG initialized to 0xF25ABFCD
[+] Time : 1.040449
[+] LBlock:        Number of right quadruples = 2^(8.144658)
[+]           Probability of right quadruples = 2^(-7.8553)

[+] RandPerm:      Number of right quadruples = 2^(1.584963)
[+]           Probability of right quadruples = 2^(-14.4150)


[+] Proc 7
[+] PRNG initialized to 0xF25ABFD3
[+] Time : 1.037820
[+] LBlock:        Number of right quadruples = 2^(8.092757)
[+]           Probability of right quadruples = 2^(-7.9072)

[+] RandPerm:      Number of right quadruples = 2^(-inf)
[+]           Probability of right quadruples = 2^(-inf)


[+] Proc 4
[+] PRNG initialized to 0xF25ABFD0
[+] Time : 1.097193
[+] LBlock:        Number of right quadruples = 2^(8.174926)
[+]           Probability of right quadruples = 2^(-7.8251)

[+] RandPerm:      Number of right quadruples = 2^(1.584963)
[+]           Probability of right quadruples = 2^(-14.4150)


[+] Proc 3
[+] PRNG initialized to 0xF25ABFCF
[+] Time : 1.044634
[+] LBlock:        Number of right quadruples = 2^(8.066089)
[+]           Probability of right quadruples = 2^(-7.9339)

[+] RandPerm:      Number of right quadruples = 2^(1.584963)
[+]           Probability of right quadruples = 2^(-14.4150)


[+] Proc 5
[+] PRNG initialized to 0xF25ABFD1
[+] Time : 1.080441
[+] LBlock:        Number of right quadruples = 2^(8.016808)
[+]           Probability of right quadruples = 2^(-7.9832)

[+] RandPerm:      Number of right quadruples = 2^(-inf)
[+]           Probability of right quadruples = 2^(-inf)


[+] Proc 2
[+] PRNG initialized to 0xF25ABFCE
[+] Time : 1.062664
[+] LBlock:        Number of right quadruples = 2^(8.169925)
[+]           Probability of right quadruples = 2^(-7.8301)

[+] RandPerm:      Number of right quadruples = 2^(2.321928)
[+]           Probability of right quadruples = 2^(-13.6781)
```




[TWINE] folder contains the search codes for generating the mixture differential distinguishers and the verification codes to verify the distinguishers we found. 

To generate the distinguishers , preinstallion is [Gurobi]. This work is done by version 10.0.

To get the results, run

```sh
g++ Twine.cpp basic.cpp -I/$GUROBI_HOME/include -L/$GUROBI_HOME/lib -lgurobi_c++ -lgurobi100 -lm -o Twine.out

nohup ./Twine.out
```

To run the verification programm, MPI package need to be installed and change the library path and include path in Makefile according to your installation. Run  `mingw32-make`  (in Windows) to compile the C++ codes.

To test 10-round by running on 8 processors, run

```sh
mpiexec -n 8 TWINE_Quadruple_MPI_RNG 10
```
You get a summary of number of quaries and number of right quadruples on each process, total queries, number of total right quadruples and the probability. We also reserved codes (commented in current version) for comparison with full round TWINE as random permutation. 
```
[+] Proc 0
[+] PRNG initialized to 0xEC25BE5E

 Quaries on each process = 2^(24.000000)
 0 proc:     Right = 2^(4.807355)
 0 proc: randRight = 2^(-inf)
 1 proc:     Right = 2^(4.523562)
 1 proc: randRight = 2^(1.000000)
 2 proc:     Right = 2^(4.643856)
 2 proc: randRight = 2^(1.584963)
 3 proc:     Right = 2^(4.954196)
 3 proc: randRight = 2^(0.000000)
 4 proc:     Right = 2^(4.906891)
 4 proc: randRight = 2^(-inf)
 5 proc:     Right = 2^(5.087463)
 5 proc: randRight = 2^(1.000000)
 6 proc:     Right = 2^(4.321928)
 6 proc: randRight = 2^(0.000000)
 7 proc:     Right = 2^(4.954196)
 7 proc: randRight = 2^(0.000000)

                              Total queries = 2^(27.000000)
 Twine:     Number of total right quadruples = 2^(7.794416)
           Probability of right quadruples  = 2^(-19.2056)
 RandPerm: Number of total right quadruples = 2^(3.321928)
            Probability of right quadruples = 2^(-23.6781)
 time on clock(): 64.176

[+] Time : 62.215294
[+] Twine:      Number of right quadruples = 2^(4.807355)
[+]           Probability of right quadruples = 2^(-19.1926)

[+] RandPerm:      Number of right quadruples = 2^(-inf)
[+]           Probability of right quadruples = 2^(-inf)


[+] Proc 4
[+] PRNG initialized to 0xEC25BE62
[+] Time : 63.278096
[+] Twine:      Number of right quadruples = 2^(4.906891)
[+]           Probability of right quadruples = 2^(-19.0931)

[+] RandPerm:      Number of right quadruples = 2^(-inf)
[+]           Probability of right quadruples = 2^(-inf)


[+] Proc 1
[+] PRNG initialized to 0xEC25BE5F
[+] Time : 64.168244
[+] Twine:      Number of right quadruples = 2^(4.523562)
[+]           Probability of right quadruples = 2^(-19.4764)

[+] RandPerm:      Number of right quadruples = 2^(1.000000)
[+]           Probability of right quadruples = 2^(-23.0000)


[+] Proc 5
[+] PRNG initialized to 0xEC25BE63
[+] Time : 62.864674
[+] Twine:      Number of right quadruples = 2^(5.087463)
[+]           Probability of right quadruples = 2^(-18.9125)

[+] RandPerm:      Number of right quadruples = 2^(1.000000)
[+]           Probability of right quadruples = 2^(-23.0000)


[+] Proc 2
[+] PRNG initialized to 0xEC25BE60
[+] Time : 64.000240
[+] Twine:      Number of right quadruples = 2^(4.643856)
[+]           Probability of right quadruples = 2^(-19.3561)

[+] RandPerm:      Number of right quadruples = 2^(1.584963)
[+]           Probability of right quadruples = 2^(-22.4150)


[+] Proc 6
[+] PRNG initialized to 0xEC25BE64
[+] Time : 64.056952
[+] Twine:      Number of right quadruples = 2^(4.321928)
[+]           Probability of right quadruples = 2^(-19.6781)

[+] RandPerm:      Number of right quadruples = 2^(0.000000)
[+]           Probability of right quadruples = 2^(-24.0000)
```



[WARP] folder contains the search codes for generating the mixture differential distinguishers and the verification codes to verify the distinguishers we found. 

To generate the distinguishers , preinstallion is [Gurobi]. This work is done by version 10.0.

To get the results, run

```sh
g++ WARP.cpp basic.cpp -I/$GUROBI_HOME/include -L/$GUROBI_HOME/lib -lgurobi_c++ -lgurobi100 -lm -o WARP.out

nohup ./WARP.out
```

To run the verification programm, MPI package need to be installed and change the library path and include path in Makefile according to your installation. Run  `mingw32-make` (in Windows) to compile the C++ codes.

To test 12-round by running on 8 processors, run

```sh
mpiexec -n 8 WARP_Quadruple_MPI_RNG 12
```
You get a summary of number of quaries and number of right quadruples on each process, total queries, number of total right quadruples and the probability. We also reserved codes (commented in current version) for comparison with full round WARP as random permutation. 
```
[+] Proc 0
[+] PRNG initialized to 0xF25A3160

 Quaries on each process = 2^(24.000000)
 0 proc:     Right = 2^(12.047124)
 0 proc: randRight = 2^(1.000000)
 1 proc:     Right = 2^(12.050189)
 1 proc: randRight = 2^(2.000000)
 2 proc:     Right = 2^(12.022021)
 2 proc: randRight = 2^(-inf)
 3 proc:     Right = 2^(12.075479)
 3 proc: randRight = 2^(0.000000)
 4 proc:     Right = 2^(12.026523)
 4 proc: randRight = 2^(-inf)
 5 proc:     Right = 2^(11.999648)
 5 proc: randRight = 2^(0.000000)
 6 proc:     Right = 2^(12.017156)
 6 proc: randRight = 2^(0.000000)
 7 proc:     Right = 2^(12.066426)
 7 proc: randRight = 2^(-inf)

                              Total queries = 2^(27.000000)
 WARP:     Number of total right quadruples = 2^(15.038276)
           Probability of right quadruples  = 2^(-11.9617)
 RandPerm: Number of total right quadruples = 2^(3.169925)
            Probability of right quadruples = 2^(-23.8301)
 time on clock(): 2171.55

[+] Time : 2167.330684
[+] WARP:      Number of right quadruples = 2^(12.047124)
[+]           Probability of right quadruples = 2^(-11.9529)

[+] RandPerm:      Number of right quadruples = 2^(1.000000)
[+]           Probability of right quadruples = 2^(-23.0000)


[+] Proc 1
[+] PRNG initialized to 0xF25A3161
[+] Time : 2162.215778
[+] WARP:      Number of right quadruples = 2^(12.050189)
[+]           Probability of right quadruples = 2^(-11.9498)

[+] RandPerm:      Number of right quadruples = 2^(2.000000)
[+]           Probability of right quadruples = 2^(-22.0000)


[+] Proc 2
[+] PRNG initialized to 0xF25A3162
[+] Time : 2171.540201
[+] WARP:      Number of right quadruples = 2^(12.022021)
[+]           Probability of right quadruples = 2^(-11.9780)

[+] RandPerm:      Number of right quadruples = 2^(-inf)
[+]           Probability of right quadruples = 2^(-inf)


[+] Proc 7
[+] PRNG initialized to 0xF25A3167
[+] Time : 2164.921349
[+] WARP:      Number of right quadruples = 2^(12.066426)
[+]           Probability of right quadruples = 2^(-11.9336)

[+] RandPerm:      Number of right quadruples = 2^(-inf)
[+]           Probability of right quadruples = 2^(-inf)


[+] Proc 4
[+] PRNG initialized to 0xF25A3164
[+] Time : 2168.334168
[+] WARP:      Number of right quadruples = 2^(12.026523)
[+]           Probability of right quadruples = 2^(-11.9735)

[+] RandPerm:      Number of right quadruples = 2^(-inf)
[+]           Probability of right quadruples = 2^(-inf)


[+] Proc 3
[+] PRNG initialized to 0xF25A3163
[+] Time : 2168.361060
[+] WARP:      Number of right quadruples = 2^(12.075479)
[+]           Probability of right quadruples = 2^(-11.9245)

[+] RandPerm:      Number of right quadruples = 2^(0.000000)
[+]           Probability of right quadruples = 2^(-24.0000)


[+] Proc 6
[+] PRNG initialized to 0xF25A3166
[+] Time : 2163.556117
[+] WARP:      Number of right quadruples = 2^(12.017156)
[+]           Probability of right quadruples = 2^(-11.9828)

[+] RandPerm:      Number of right quadruples = 2^(0.000000)
[+]           Probability of right quadruples = 2^(-24.0000)


[+] Proc 5
[+] PRNG initialized to 0xF25A3165
[+] Time : 2165.254359
[+] WARP:      Number of right quadruples = 2^(11.999648)
[+]           Probability of right quadruples = 2^(-12.0004)

[+] RandPerm:      Number of right quadruples = 2^(0.000000)
[+]           Probability of right quadruples = 2^(-24.0000)
```