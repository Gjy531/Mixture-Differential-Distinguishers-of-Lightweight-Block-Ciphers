/*
all:
	g++ SKINNY_Quadruple_MPI_RNG.cpp --std=c++11 -Wall -O3 -o SKINNY_Quadruple_MPI_RNG -l msmpi -L "C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" -I "C:\Program Files (x86)\Microsoft SDKs\MPI\Include"
    mpiexec -n 8 SKINNY_Quadruple_MPI_RNG 7
clean:
	rm -f *.o SKINNY_Quadruple_MPI
 * Quadruple distinguisher verification for Skinny
 * Date: Apr 14, 2023
 * Author: Kexin Qiao
 * Contact: qiao.kexin@bit.edu.cn
*/
// Skinny Cipher 
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

//Skinny-64-64: 32 rounds
// 4-bit Sbox
const uint8_t sbox_4[16] = {12,6,9,0,1,10,2,11,3,8,5,13,4,14,7,15};

// ShiftAndSwitchRows permutation
const uint8_t P[16] = {0,1,2,3,7,4,5,6,10,11,8,9,13,14,15,12};

// Tweakey permutation
const uint8_t TWEAKEY_P[16] = {9,15,8,13,10,14,12,11,0,1,2,3,4,5,6,7};

// round constants
const uint8_t RC[62] = {
		0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3E, 0x3D, 0x3B, 0x37, 0x2F,
		0x1E, 0x3C, 0x39, 0x33, 0x27, 0x0E, 0x1D, 0x3A, 0x35, 0x2B,
		0x16, 0x2C, 0x18, 0x30, 0x21, 0x02, 0x05, 0x0B, 0x17, 0x2E,
		0x1C, 0x38, 0x31, 0x23, 0x06, 0x0D, 0x1B, 0x36, 0x2D, 0x1A,
		0x34, 0x29, 0x12, 0x24, 0x08, 0x11, 0x22, 0x04, 0x09, 0x13,
		0x26, 0x0c, 0x19, 0x32, 0x25, 0x0a, 0x15, 0x2a, 0x14, 0x28,
		0x10, 0x20};

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

void initialize(uint8_t k[16], uint8_t tk[][16]) {
	for (uint8_t i = 0; i < 16; i++)
		tk[0][i] = (k[i] & 0xf);
    for (uint8_t j = 1; j < 32 ; j++)
        for (uint8_t i = 0; i < 16; i++)
		    tk[j][i] = tk[j-1][TWEAKEY_P[i]];
}

void enc(uint8_t R, uint8_t plaintext[16], uint8_t ciphertext[16], uint8_t tk[][16]) {
    
    for (uint8_t i = 0; i < 16; i++) {
        ciphertext[i] = plaintext[i] & 0xf;
    }
   
    for (uint8_t r = 0; r < R; r++) {    
  
        //SubCells(SC)
        for (uint8_t i = 0; i < 16; i++)
                ciphertext[i] = sbox_4[ciphertext[i]];
       
        //AddConstants(AC)
        ciphertext[0] ^= (RC[r] & 0xf);
	    ciphertext[4] ^= ((RC[r]>>4) & 0x3);
	    ciphertext[8] ^= 0x2;
        
        //AddRoundTweakey(ART)
        for (uint8_t i = 0; i < 8; i++) {            
            ciphertext[i] ^= tk[r][i];
        }

        //ShiftRows(SR)
        uint8_t Temp[16];
        for (uint8_t i = 0; i < 16; i++)
                Temp[i] = ciphertext[P[i]];  
  
        //MixColumns(MC)
        for (uint8_t i = 0; i < 4; i++) {
            ciphertext[i+12] = Temp[i]^Temp[i+8];
            ciphertext[i+8] = Temp[i+4]^Temp[i+8];
            ciphertext[i+4] = Temp[i];
            ciphertext[i] = ciphertext[i+12]^Temp[i+12];
        }
    }
}

void genQuadruple(uint8_t rd, uint8_t p0[16], uint8_t p1[16], uint8_t p2[16], uint8_t p3[16], string inpatt_state){
    int i;
    uint8_t randcell0, randcell1,randcell2,randcell3;
    
    // fix a nonzero difference for four columns
    randcell0= dis2(gen);
    randcell1 = dis2(gen);
    randcell2 = dis2(gen);
    randcell3 = dis2(gen);
    switch (rd){
        case 8: case 10:
            randcell1 = randcell0;
            break;
        default:
            break;
    }
    
    //randcell = 0xA;
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
                
               switch (i){
                    case 0: case 4: case 8: case 12:
                        p2[i] = p0[i] ^ randcell0;
                        break;
                    case 1: case 5: case 9: case 13:
                        p2[i] = p0[i] ^ randcell1;
                        break;
                    case 2: case 6: case 10: case 14:
                        p2[i] = p0[i] ^ randcell2;
                        break;
                    default:
                        p2[i] = p0[i] ^ randcell3;
               }
                
                p3[i] = p2[i];
                break;
            case 'c':
                p0[i] = dis(gen);
                switch (i){
                    case 0: case 4: case 8: case 12:
                        p1[i] = p0[i] ^ randcell0;
                        break;
                    case 1: case 5: case 9: case 13:
                        p1[i] = p0[i] ^ randcell1;
                        break;
                    case 2: case 6: case 10: case 14:
                        p1[i] = p0[i] ^ randcell2;
                        break;
                    default:
                        p1[i] = p0[i] ^ randcell3;
                }
                
                p2[i] = p0[i];
                p3[i] = p1[i];
                break;
            case 'x':
                p0[i] = dis(gen);
                switch (i){
                    case 0: case 4: case 8: case 12:
                        p1[i] = p0[i] ^ randcell0;
                        break;
                    case 1: case 5: case 9: case 13:
                        p1[i] = p0[i] ^ randcell1;
                        break;
                    case 2: case 6: case 10: case 14:
                        p1[i] = p0[i] ^ randcell2;
                        break;
                    default:
                        p1[i] = p0[i] ^ randcell3;
                }
                
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
UINT64 * quadruple(uint8_t rd, uint8_t tk[][16], UINT64 N3, string inpatt, string outpatt){
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
        genQuadruple(rd, p0, p1, p2, p3, inpatt);
        
        
        enc(rd, p0, c0, tk);
        enc(rd, p1, c1, tk);
        enc(rd, p2, c2, tk);
        enc(rd, p3, c3, tk);
        
        if (check_pattern(c0,c1,c2,c3,outpatt))
            count[0]++;
        
        enc(32, p0, c0, tk);
        enc(32, p1, c1, tk);
        enc(32, p2, c2, tk);
        enc(32, p3, c3, tk);
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
    uint8_t k[16];		    
    uint8_t tk[32][16];
    UINT64 bnum=0;
    static UINT64 count[2];
    UINT64 *count_itr;
    count[0] = 0;
    count[1] = 0;
    while (bnum < N2){
        bnum++;
        //generate a random key
        for(int i = 0; i < 16; i++) k[i] = dis(gen);
        initialize(k, tk);
        count_itr = quadruple(rd, tk, N3, inpatt, outpatt);
        count[0] += *count_itr;
        count[1] += *(count_itr + 1);
    }
    return count;
}

int main(int argc, char** argv){
    
    UINT64 N2 = 1; //= 1 << 4; //test under N2 keys
    UINT64 N3 = 1;;// = 1 << 10; //generate N3 quadruples under each key
    uint8_t rd = 7;
    string inpatt;
    string outpatt;
    
    switch (atoi(argv[1])) {
        /*case 6:
        {
            rd = 6;
            inpatt = "xs------xs------";
            outpatt = "xs**--xs**sx****";
            N2 = 1 << 4; //test under N2 keys
            N3 = 1 << 4; //generate N3 quadruples under each key
        }
            break;*/
        case 7:
        {
            rd = 7;
            inpatt = "------------s--c";
            outpatt = "****c*******c***";
            N2 = 1 << 8; //test under N2 keys
            N3 = 1 << 8; //generate N3 quadruples under each key
        }
            break;
        /*case 8:
        {
            rd = 8;
            inpatt = "-x--xx-c-c---x--";
            outpatt = "******c*********";
            N2 = 1 << 8; //test under N2 keys
            N3 = 1 << 8; //generate N3 quadruples under each key
        }
            break;
        case 9:
        {
            rd = 9;
            inpatt = "----xs----------";
            outpatt = "c*****cc******c*";
            N2 = 1 << 14; //test under N2 keys
            N3 = 1 << 14; //generate N3 quadruples under each key
        }
            break;
        case 10:
        {
            rd = 10;
            inpatt = "sxssccs----ssxs-";
            outpatt = "s*****ss******sx";
            N2 = 1 << 16; //test under N2 keys
            N3 = 1 << 16; //generate N3 quadruples under each key
        }
            break;*/
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
        printf("Skinny:     Number of total right quadruples = 2^(%f)\n", log(right)/log(2));
        printf("           Probability of right quadruples  = 2^(-%0.4f)\n", (log(N2 * N3 * world_size)-log(right))/log(2));
        printf(" RandPerm: Number of total right quadruples = 2^(%f)\n", log(rand_right)/log(2));
        printf("            Probability of right quadruples = 2^(-%0.4f)\n", (log(N2 * N3 * world_size)-log(rand_right))/log(2));
        cout << " time on clock(): " << ((double)clock() - clock_timer) / CLOCKS_PER_SEC<<endl<<endl;
    }

    
    printf("[+] Time : %lf\n",proc_time);
    printf("[+] Skinny:      Number of right quadruples = 2^(%f)\n", log(*count)/log(2));
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