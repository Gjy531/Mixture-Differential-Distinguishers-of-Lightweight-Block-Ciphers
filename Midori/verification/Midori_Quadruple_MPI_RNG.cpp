/*
all:
	g++ Midori_Quadruple_MPI_RNG.cpp --std=c++11 -Wall -O3 -o Midori_Quadruple_MPI_RNG -l msmpi -L "C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" -I "C:\Program Files (x86)\Microsoft SDKs\MPI\Include"
    mpiexec -n 8 Midori_Quadruple_MPI_RNG 4
clean:
	rm -f *.o _Quadruple_MPI
 * Quadruple distinguisher verification for Midori 
*/
//#define SDL_MAIN_HANDLED
#define __USE_MINGW_ANSI_STDIO 1  // to avoid %ll issue. special request for windows
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string>
#include <math.h>
#include <mpi.h>
#include <iostream>
#include <assert.h>
#include <random>
using namespace std;

typedef unsigned long long int UINT64;

std::mt19937 gen;
std::uniform_int_distribution<int> dis(0x0,0xf);
std::uniform_int_distribution<int> dis2(0x1,0xf);

//Midori64: 16 rounds
// 4-bit S-box
const uint8_t Sb0[16] = {0xc,0xa,0xd,0x3,0xe,0xb,0xf,0x7,0x8,0x9,0x1,0x5,0x0,0x2,0x4,0x6};

// Permutation
const uint8_t P[16] = {0,10,5,15,14,4,11,1,9,3,12,6,7,13,2,8};

// round constants for round keys
const uint8_t alpha[15][16] = {
		0x0,0x0,0x0,0x1,0x0,0x1,0x0,0x1,0x1,0x0,0x1,0x1,0x0,0x0,0x1,0x1,
        0x0,0x1,0x1,0x1,0x1,0x0,0x0,0x0,0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,
        0x1,0x0,0x1,0x0,0x0,0x1,0x0,0x0,0x0,0x0,0x1,0x1,0x0,0x1,0x0,0x1,
        0x0,0x1,0x1,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x1,0x1,
        0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x1,0x1,0x1,0x1,
        0x1,0x1,0x0,0x1,0x0,0x0,0x0,0x1,0x0,0x1,0x1,0x1,0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x1,0x1,0x0,0x0,0x1,0x1,0x0,
        0x0,0x0,0x0,0x0,0x1,0x0,0x1,0x1,0x1,0x1,0x0,0x0,0x1,0x1,0x0,0x0,
        0x1,0x0,0x0,0x1,0x0,0x1,0x0,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x1,
        0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x1,0x1,0x1,0x0,0x0,0x0,
		0x0,0x1,0x1,0x1,0x0,0x0,0x0,0x1,0x1,0x0,0x0,0x1,0x0,0x1,0x1,0x1,
        0x0,0x0,0x1,0x0,0x0,0x0,0x1,0x0,0x1,0x0,0x0,0x0,0x1,0x1,0x1,0x0,
        0x0,0x1,0x0,0x1,0x0,0x0,0x0,0x1,0x0,0x0,0x1,0x1,0x0,0x0,0x0,0x0,
        0x1,0x1,0x1,0x1,0x1,0x0,0x0,0x0,0x1,0x1,0x0,0x0,0x1,0x0,0x1,0x0,
        0x1,0x1,0x0,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x0,0x1,0x0,0x0,0x0,0x0};

void print_state(uint8_t state[16])
{
    for (int i = 0; i < 16; i++)
        printf("%01x", state[i]);
    printf("\n");
}

void init_prng_mt(int offset){
    unsigned int initial_seed = 10*time(NULL) + offset;
    gen.seed(initial_seed);
    printf("[+] PRNG initialized to 0x%08X\n", initial_seed);
}

void initialize(uint8_t K[32], uint8_t WK[], uint8_t RK[][16]) {
    uint8_t KEY[2][16];
	for (uint8_t i = 0; i < 16; i++)
    {
        KEY[0][i] = (K[i] & 0xf);
        KEY[1][i] = (K[i+16] & 0xf);
        WK[i] = KEY[0][i]^KEY[1][i];
    }
		
    for (uint8_t j = 0; j < 15 ; j++)
        for (uint8_t i = 0; i < 16; i++)
		    RK[j][i] = KEY[j%2][i] ^ alpha[j][i];
}

void enc(uint8_t R, uint8_t plaintext[16], uint8_t ciphertext[16], uint8_t RK[][16]) {
    
    for (uint8_t i = 0; i < 16; i++) {
        ciphertext[i] = plaintext[i] & 0xf;
    }
   
    for (uint8_t r = 0; r < R; r++) {    
  
        //SubCell
        for (uint8_t i = 0; i < 16; i++)
                ciphertext[i] = Sb0[ciphertext[i]];
       
        //ShuffleCell
        uint8_t Temp[16];
        for (uint8_t i = 0; i < 16; i++)
                Temp[i] = ciphertext[P[i]]; 

        //MixColumn
        uint8_t Temp1[4];
        uint8_t Temp2[4];
        for (uint8_t i = 0; i < 4; i++) {
            Temp1[i] = Temp[4*i]^Temp[4*i+1];
            Temp2[i] = Temp[4*i+2]^Temp[4*i+3];
            ciphertext[4*i] = Temp2[i]^Temp[4*i+1];
            ciphertext[4*i+1] = Temp2[i]^Temp[4*i];
            ciphertext[4*i+2] = Temp1[i]^Temp[4*i+3];
            ciphertext[4*i+3] = Temp1[i]^Temp[4*i+2];
        }
        
        //KeyAdd
        for (uint8_t i = 0; i < 16; i++) {            
            ciphertext[i] ^= RK[r][i];
        }
    }
}

void genQuadruple(uint8_t p0[16], uint8_t p1[16], uint8_t p2[16], uint8_t p3[16], string inpatt_state){
    int i;
    
    for (i = 0; i < 16; i++){
        switch (inpatt_state[i]){
            case '-':
                p0[i] = dis(gen);
                p1[i] = p0[i];
                p2[i] = p0[i];
                p3[i] = p0[i];
                break;
            case 's':
                p0[i] = dis(gen);
                p1[i] = p0[i];
                p2[i] = p0[i] ^ dis2(gen);
                p3[i] = p2[i];
                break;
            case 'c':
                p0[i] = dis(gen);
                p1[i] = p0[i] ^ dis2(gen);
                p2[i] = p0[i];
                p3[i] = p1[i];
                break;
            case 'x':
                p0[i] = dis(gen);
                p1[i] = p0[i] ^ dis2(gen);
                p2[i] = p1[i];
                p3[i] = p0[i];
                break;
            default: // '*'
                p0[i] = dis(gen);
                p1[i] = dis(gen);
                p2[i] = dis(gen);
                p3[i] = dis(gen);
                break;
        }
    }

    
}


bool check_pattern(uint8_t c0[16], uint8_t c1[16], uint8_t c2[16], uint8_t c3[16], string patt){
    /*
    check if quadruple (c0,c1,c2,c3) satisfy patt.
    */
    bool flag = true;
    for (int i =0; i<16; i++){
        switch (patt[i]){
            case '-':
                if ((c0[i]==c1[i]) and (c0[i]==c2[i]) and (c0[i]==c3[i]) )
                    break;
                else{
                    flag = false;
                    break;
                }
            case 'c':
                if ((c0[i]==c2[i]) and (c1[i]==c3[i]))
                
                    break;
                else{
                    flag = false;
                    break;
                }
            case 'x':
                if ((c0[i]==c3[i]) and (c1[i]==c2[i]))
                    break;
                else{
                    flag = false;
                    break;
                }
            case 's':
                if ((c0[i]==c1[i]) and (c2[i]==c3[i]))
                    break;
                else{
                    flag = false;
                    break; 
                }   
            default:
                break;     
        }
    }
    return flag;
}
UINT64 * quadruple(uint8_t rd, uint8_t WK[], uint8_t RK[][16], UINT64 N3, string inpatt, string outpatt){
    /*
    Construct N3 quadruples in inpatt under a fixed tweakey.
    Return the number of right quadruples satisfying outpatt.
    */
    uint8_t p0[16],p1[16];
    uint8_t p2[16],p3[16];
    uint8_t c0[16],c1[16];
    uint8_t c2[16],c3[16];
    //bool debug = false;
    UINT64 bnum=0;
    static UINT64 count[2];
    count[0] = 0;
    count[1] = 0;
    while (bnum < N3){
        bnum++;
        genQuadruple(p0, p1, p2, p3, inpatt);
        
        for (uint8_t i = 0; i < 16; i++)
        {
            p0[i] ^= WK[i]; 
            p1[i] ^= WK[i];
            p2[i] ^= WK[i];
            p3[i] ^= WK[i];
        }
        
        
        enc(rd, p0, c0, RK);
        enc(rd, p1, c1, RK);
        enc(rd, p2, c2, RK);
        enc(rd, p3, c3, RK);
        
        if (check_pattern(c0,c1,c2,c3,outpatt))
            count[0]++;
        
        enc(15, p0, c0, RK);
        enc(15, p1, c1, RK);
        enc(15, p2, c2, RK);
        enc(15, p3, c3, RK);

        for (uint8_t i = 0; i < 16; i++)
        {
            c0[i] = Sb0[c0[i]] ^ WK[i];
            c1[i] = Sb0[c1[i]] ^ WK[i];
            c2[i] = Sb0[c2[i]] ^ WK[i];
            c3[i] = Sb0[c3[i]] ^ WK[i];
        }

        if (check_pattern(c0,c1,c2,c3,outpatt))
            count[1]++;
        
    }
    return count;
}

UINT64 * testOnMultKey(uint8_t rd, UINT64 N2, UINT64 N3, string inpatt, string outpatt){
    /*
    Set N2 multi masterkeys. Under each masterkey, test on N3 quadruples.
    Return sum of right quadruple numbers under each key.
    */
    uint8_t K[32];		    
    uint8_t RK[15][16];
    uint8_t WK[16];
    UINT64 bnum=0;
    static UINT64 count[2];
    UINT64 *count_itr;
    count[0] = 0;
    count[1] = 0;
    while (bnum < N2){
        bnum++;
        //generate a random key
        for(int i = 0; i < 32; i++) K[i] = dis(gen);
        initialize(K,WK,RK);
        count_itr = quadruple(rd, WK, RK, N3, inpatt, outpatt);
        count[0] += *count_itr;
        count[1] += *(count_itr + 1);
    }
    return count;
}

int main(int argc, char** argv){
    
    UINT64 N2 = 1; //= 1 << 4; //test under N2 keys
    UINT64 N3 = 1;;// = 1 << 10; //generate N3 quadruples under each key
    uint8_t rd = 4;
    string inpatt;
    string outpatt;
    
    switch (atoi(argv[1])) {
        case 4:
        {
            rd = 4;
            inpatt = "---s---c-----c--";
            outpatt = "xxxx************";
            N2 = 1 << 16; //test under N2 keys
            N3 = 1 << 16; //generate N3 quadruples under each key
        }
            break;
        default:
            cout<<"Not supported."<<endl;
            return 0;
    }
    
    

    /*
    For each processor, run 
    testOnMultKey(uint8_t rd, int64_t N2, int64_t N3, string inpatt, string outpatt):
    Set N2 multi masterkeys. Under each masterkey, test on N3 quadruples.
    Return sum of right quadruple numbers under each key.
    */
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    printf("\n[+] Proc %d\n", world_rank);
    init_prng_mt(world_rank);
    //Compute on each process
    UINT64 *count;
    
    clock_t clock_timer;
    clock_timer = clock();
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    
    //init_prng_mt(world_rank);
    double proc_time;
    
    proc_time = MPI_Wtime();
    count = testOnMultKey(rd,N2,N3,inpatt,outpatt);
    proc_time = MPI_Wtime() - proc_time;
    
    MPI_Barrier(MPI_COMM_WORLD);
    //Gather all number of right quadruples down to the root process
    UINT64 *NUM = NULL;
    if (world_rank == 0){
        NUM = (UINT64 *)malloc(world_size * sizeof(UINT64) * 2);
        assert(NUM != NULL);
    }

    MPI_Gather(count, 2, MPI_UNSIGNED_LONG_LONG, NUM, 2, MPI_UNSIGNED_LONG_LONG,0,MPI_COMM_WORLD);

    //Compute sum on the root process
    if (world_rank == 0){
        UINT64 right,rand_right;
        right = 0;
        rand_right = 0;
        printf("\n Quaries on each process = 2^(%f)\n",log(N2 * N3)/log(2));
        for (int i = 0; i < world_size *2; i=i+2){
            
            right += *(NUM + i);
            rand_right += *(NUM + i + 1);
            printf(" %d proc:     Right = 2^(%f)\n", i/2, log(*(NUM + i))/log(2));
            printf(" %d proc: randRight = 2^(%f)\n", i/2, log(*(NUM + i + 1))/log(2));
        }
        printf("\n                              Total queries = 2^(%f) \n", log(N2 * N3 * world_size)/log(2));
        printf("Midori:     Number of total right quadruples = 2^(%f)\n", log(right)/log(2));
        printf("           Probability of right quadruples  = 2^(-%0.4f)\n", (log(N2 * N3 * world_size)-log(right))/log(2));
        printf(" RandPerm: Number of total right quadruples = 2^(%f)\n", log(rand_right)/log(2));
        printf("            Probability of right quadruples = 2^(-%0.4f)\n", (log(N2 * N3 * world_size)-log(rand_right))/log(2));
        cout << " time on clock(): " << ((double)clock() - clock_timer) / CLOCKS_PER_SEC<<endl<<endl;
    }

    
    printf("[+] Time : %lf\n",proc_time);
    printf("[+] Midori:      Number of right quadruples = 2^(%f)\n", log(*count)/log(2));
    printf("[+]           Probability of right quadruples = 2^(-%0.4f)\n\n", (log(N2 * N3)-log(*count))/log(2));
    printf("[+] RandPerm:      Number of right quadruples = 2^(%f)\n", log(*(count+1))/log(2));
    printf("[+]           Probability of right quadruples = 2^(-%0.4f)\n\n", (log(N2 * N3)-log(*(count+1)))/log(2));

    //Clean up
    if (world_rank == 0){
        free(NUM);
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}
