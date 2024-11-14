#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>
#include <stddef.h>

#define LL_BITS 0xC0 // 1100 0000
#define LM_BITS 0x30 // 0011 0000
#define RM_BITS 0x0C // 0000 1100
#define RR_BITS 0x03 // 0000 0011

long set_bound();
void find_primes(long bound, int verbose, char choice, int binary);


int main(int argc, char * argv[])
{
    long bound=100;
    char choice='p';
    int opt;
    int verbose=0;
    int binary=0;
    while((opt = getopt(argc, argv, "pcu:bvh")) != -1)
    {
        
        switch(opt)
        {
            case 'p':   //Outputs the prime numbers up to bound
                choice='p';
                break;

            case 'c':   //Outputs composite numbers up to bound
                choice='c';
                break;

            case 'u':   //Sets the bound to input
                if(isdigit(optarg)==0)
                {
                    return EXIT_FAILURE;
                }
                bound=(long)optarg;
                break;
            case 'b':
                binary=1;
                break;
            case 'v':
                verbose=1;
                break;
            case 'h':
                break;
            default:
                return 0;
        }
    }
    find_primes(bound, verbose, choice, binary);

    return EXIT_SUCCESS;
}

void find_primes(long bound, int verbose, char choice, int binary)
{
    uint8_t *list=malloc(((bound/4)+1));
    int count=2;
    int current=2;
    int currentprime=current;
    if(binary==1)   //remove before submitting
        binary=1;
    if(verbose==1)
    {
        fprintf(stderr,"upper bound: %ld\nshow primes: %s\n", bound, choice=='p' ? "true" : "false");
    }
    while(currentprime< sqrt(bound))
    {
        
        while(current<=bound)
        {
            uint8_t bits = 0x00;
            current=count*currentprime; //prime*n, to mark each composite
            switch ( current % 4 )  //Sets the bits to mark composite numbers
            {
                case 0:
                    bits = LL_BITS;
                    break;
                case 1:
                    bits = LM_BITS;
                    break;
                case 2:
                    bits = RM_BITS;
                    break;
                case 3:
                    bits = RR_BITS;
                    break;
            }
            list[current/4] |= bits;
            ++count;
        }
    }
    switch(choice)
    {
        case 'p':
            break;
        case 'c':
            break;
    }
    return;
}
