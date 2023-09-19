/*
all:
	g++ LBlock_Quadruple_MPI_RNG.cpp -o LBlock_Quadruple_MPI_RNG -fopenmp -l msmpi -L "C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64" -I "C:\Program Files (x86)\Microsoft SDKs\MPI\Include"
    mpiexec -n 8 LBlock_Quadruple_MPI_RNG 9
clean:
	rm -f *.o WARP_Quadruple_MPI
 * Quadruple distinguisher verification for LBlock
*/
// WARP Cipher 
//#define SDL_MAIN_HANDLED
#define __USE_MINGW_ANSI_STDIO 1  // to avoid %ll issue. special request for windows
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string>
#include <math.h>
#include <iostream>
#include <assert.h>
#include <random>
#include <mpi.h>
using namespace std;

typedef unsigned long long int UINT64;

std::mt19937 gen;
std::uniform_int_distribution<int> dis(0x0,0xf);
std::uniform_int_distribution<int> dis2(0x1,0xf);

const uint8_t S[10][16]={{14,9,15,0,13,4,10,11,1,2,8,3,7,6,12,5},
{4,11,14,9,15,13,0,10,7,12,5,6,2,8,1,3},
{1,14,7,12,15,13,0,6,11,5,9,3,2,4,8,10},
{7,6,8,11,0,15,3,14,9,10,12,13,5,2,4,1},
{14,5,15,0,7,2,12,13,1,8,4,9,11,10,6,3},
{2,13,11,12,15,14,0,9,7,10,6,3,1,8,4,5},
{11,9,4,14,0,15,10,13,6,12,5,7,3,8,1,2},
{13,10,15,0,14,4,9,11,2,1,8,3,7,5,12,6},
{8,7,14,5,15,13,0,6,11,12,9,10,2,4,1,3},
{11,5,15,0,7,2,9,13,4,8,1,12,14,10,3,6}};
const uint8_t P[8]={1,3,0,2,5,7,4,6};

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

void roundkey(unsigned int k[5],unsigned int rk[32] ){
    unsigned int i,t1,t2,t3,t4,t0,c1,c2;
    rk[0]=(k[0]<<16)^ k[1];
    for(i=1;i<32;i++){
        // 32 left shift (then 3 right shift)
        t1=k[0]; t2=k[1];
        k[0]=k[2];  k[1]=k[3]; k[2]=k[4]; k[3]=t1; k[4]=t2;
        //3 right shift
        t0=(k[0]&0x7);              t1=(k[1]&0x7);              t2=(k[2]&0x7);
        t3=(k[3]&0x7);              t4=(k[4]&0x7);
        k[4]=(k[4]>>3)^(t3<<13);
        k[3]=(k[3]>>3)^(t2<<13);
        k[2]=(k[2]>>3)^(t1<<13);
        k[1]=(k[1]>>3)^(t0<<13);
        k[0]=(k[0]>>3)^(t4<<13);
        //s-box
        t1=(k[0]>>12)&0xF;
        t2=(k[0]>>8)&0xF;
        t1=S[9][t1];
        t2=S[8][t2];
        k[0]=(t1<<12)^(t2<<8)^(k[0]&0x00FF);
        //counter
        c1=i&0x3; c2=i>>2;
        k[2]^=c1<<14; k[1]^=c2;
        //get roundkey
        rk[i]=(k[0]<<16)^ k[1];
    }
}

unsigned int S_Layer(unsigned int x){
    unsigned int temp=0x0;
    int i;
    for(i=0;i<7;i++){
        temp^=S[7-i][(x>>(28-4*i))&0xF];
        temp<<=4;
    }                               
    temp^=S[7-i][x&0xF];
    return temp;
}

unsigned int P_Layer(unsigned int x){
    unsigned short temp[8],i;
    unsigned int t=0x0;
    
    for(i=0;i<8;i++)
        temp[i]=(x>>(28-(4*i)))&0xF;
    
    for(i=0;i<7;i++){
        t^=temp[P[i]];
        t<<=4;
    }
    t^=temp[P[i]];
    
    return t;
}

unsigned int F(unsigned int x, unsigned int k){
    x^=k;
    x=S_Layer(x);
    x=P_Layer(x);
    return x;
}
void swap(unsigned int *left, unsigned int *right){
    unsigned int temp;
    temp=(*left);
    (*left)=(*right);
    (*right)=temp;
}

void enc(uint8_t R, uint8_t plaintext[16], uint8_t ciphertext[16], unsigned int rk1[32]){
    unsigned int left, right;
    long long ptext,ctext;
    int i;
    
    ptext = 0;
    for (i=0;i < 15; i++){
        ptext ^= (plaintext[i] & 0xf);
        ptext <<= 4;
         
    }
    ptext ^= (plaintext[i] & 0xf);
    
    left=ptext>>32;
    right=ptext&0xFFFFFFFF;
    
    for(i=0; i<R; i++){
        
        right=(right<<8)^(right>>24); 
        right^=F(left,rk1[i]);
        swap(&left,&right);
    }
    
    if (i==32){
        
        ctext=right;
        ctext<<=32;
        ctext^=left;
    }else{
        
        ctext = left;
        ctext<<=32;
        ctext ^= right;
    }

    for (i=0;i<16;i++){
        ciphertext[i] = (ctext>>((15-i)*4)) & 0xf;
        
    }
    
}


void genQuadruple(uint8_t p0[16], uint8_t p1[16], uint8_t p2[16], uint8_t p3[16], string inpatt_state){
    int i;
    
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
UINT64 * quadruple(uint8_t rd, unsigned int rk[], UINT64 N3, string inpatt, string outpatt){
    /*
    Construct N3 quadruples in inpatt under a fixed tweakey.
    Return the number of right quadruples satisfying outpatt.
    */
    uint8_t p0[16],p1[16];
    uint8_t p2[16],p3[16];
    uint8_t c0[16],c1[16];
    uint8_t c2[16],c3[16];
    
    UINT64 bnum=0;
    static UINT64 count[2];
    count[0] = 0;
    count[1] = 0;
    while (bnum < N3){
        bnum++;
        genQuadruple(p0, p1, p2, p3, inpatt);
        
        
        enc(rd, p0, c0, rk);
        enc(rd, p1, c1, rk);
        enc(rd, p2, c2, rk);
        enc(rd, p3, c3, rk);

        
        if (check_pattern(c0,c1,c2,c3,outpatt))
            count[0]++;
        
        enc(32, p0, c0, rk);
        enc(32, p1, c1, rk);
        enc(32, p2, c2, rk);
        enc(32, p3, c3, rk);
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
    unsigned int key[5]; // Key 
    unsigned int temp;    
    
    unsigned int rk[32];
    UINT64 bnum=0;
    static UINT64 count[2];
    UINT64 *count_itr;
    count[0] = 0;
    count[1] = 0;
    
    while (bnum < N2){
        bnum++;
        //generate a random key
        
        for(int i = 0; i < 5; i++) {
            temp = 0;
            temp ^= dis(gen);
            temp <<=12;
            temp ^= dis(gen);
            temp <<= 8;
            temp ^= dis(gen);
            temp <<=4;
            temp ^= dis(gen);
            key[i] = temp;
            
        }
        roundkey(key,rk);
        
        count_itr = quadruple(rd, rk, N3, inpatt, outpatt);
        count[0] += *count_itr;
        count[1] += *(count_itr + 1);
    }
    return count;
}

int main(int argc, char** argv){
    
    UINT64 N2 = 1; //= 1 << 4; //test under N2 keys
    UINT64 N3 = 1;// = 1 << 10; //generate N3 quadruples under each key
    uint8_t rd = 1;
    string inpatt;
    string outpatt;
    
    switch (atoi(argv[1])) {
        case 9:
        {
            rd = 9;
            inpatt = "--------xxs-----";
            outpatt = "**********s****s";
            N2 = 1 << 8; //test under N2 keys
            N3 = 1 << 8; //generate N3 quadruples under each key
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
        printf("LBlock:    Number of total right quadruples = 2^(%f)\n", log(right)/log(2));
        printf("           Probability of right quadruples  = 2^(-%0.4f)\n", (log(N2 * N3 * world_size)-log(right))/log(2));
        printf(" RandPerm: Number of total right quadruples = 2^(%f)\n", log(rand_right)/log(2));
        printf("            Probability of right quadruples = 2^(-%0.4f)\n", (log(N2 * N3 * world_size)-log(rand_right))/log(2));
        cout << " time on clock(): " << ((double)clock() - clock_timer) / CLOCKS_PER_SEC<<endl<<endl;
    }

    
    printf("[+] Time : %lf\n",proc_time);
    printf("[+] LBlock:        Number of right quadruples = 2^(%f)\n", log(*count)/log(2));
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