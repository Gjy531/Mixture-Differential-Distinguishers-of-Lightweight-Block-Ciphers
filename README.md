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

To run the verification programm, MPI package need to be installed and change the library path and include path in Makefile according to your installation. Run mingw32-make (in Windows) to compile the C++ codes.

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







[MANTIS] This repository contains two folders: [MILP](https://github.com/shenshensunshine/LightweightMixtureDifferentialCipher-Mantis/tree/main/MILP) and [Verification](https://github.com/shenshensunshine/LightweightMixtureDifferentialCipher-Mantis/tree/main/Verification). [MILP](https://github.com/shenshensunshine/LightweightMixtureDifferentialCipher-Mantis/tree/main/MILP) folder contains source codes to search for the mixture differential distinguishers covering 4-  6- and 8- round of Mantis. The codes are launched on Linux OS. To run our codes preinstallation is [Gurobi](https://www.gurobi.com/) for solving constraint programming problems. To get the 15 mixture trails for 5-round Mantis, run

+ `g++ Mantis1.cpp basic.cpp -I/$GUROBI_HOME/include -L/$GUROBI_HOME/lib -lgurobi_c++ -lgurobi100 -lm -o Mantis1.out`
+ `nohup ./Mantis1.out`

[Verification](https://github.com/shenshensunshine/LightweightMixtureDifferentialCipher-Mantis/tree/main/Verification) folder contains the verification codes to verify the quadruple distinguishers we found. To run the programm, MPI package need to be installed and change the library path and include path in Makefile according to your installation path.  Run `mingw32-make` (in Windows) to compile the C++ codes. 

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







[TWINE] folder contains the search codes for generating the mixture differential distinguishers and the verification codes to verify the distinguishers we found. 

To generate the distinguishers , preinstallion is [Gurobi]. This work is done by version 10.0.

To get the results, run

```sh
g++ Twine.cpp basic.cpp -I/$GUROBI_HOME/include -L/$GUROBI_HOME/lib -lgurobi_c++ -lgurobi100 -lm -o Twine.out

nohup ./Twine.out
```

To run the verification programm, MPI package need to be installed and change the library path and include path in Makefile according to your installation. Run mingw32-make (in Windows) to compile the C++ codes.

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