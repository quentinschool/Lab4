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
#include <stdbool.h>

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
    long primecount=1;
    long *primes=malloc(bound);
    long compositescount=0;
    long *composites=malloc(bound);
    uint8_t *list=malloc(((bound/4)+1));
    long count=2;            
    long current=2;          //current number being checked
    long currentprime=current;
    long i=0;
    uint8_t bits = 0x00;    //for setting and checking bits
    bool is_set=true;       //for checking if bit is set
    primes[0]=2;

    if(binary==1)   //remove before submitting
        binary=1;
    if(verbose==1)
    {
        fprintf(stderr,"upper bound: %ld\nshow primes: %s\n", bound, choice=='p' ? "true" : "false");
    } 
    count=2;
    
    
    while(currentprime< sqrt(bound))
    {
        current=currentprime;
        count=2;
        if(verbose==true)
            fprintf(stderr, "marking multiples of %ld\n", currentprime);
        while(current<=bound)
        {
            current=count*currentprime; //prime*n, to mark each composite

            bits = 0x00;
            
            switch ( current % 4 )  //Sets the bits to mark composite numbers
            {
                case 0:
                    if(list[current/4] & LL_BITS)
                        break;
                    if(verbose==true && ~(list[current/4] & LL_BITS))
                        fprintf(stderr, "\tmarked %ld\n", current);
                    bits = LL_BITS;
                    break;
                case 1:
                    if(list[current/4] & LM_BITS)
                        break;
                    if(verbose==true && ~(list[current/4] & LM_BITS))
                        fprintf(stderr, "\tmarked %ld\n", current);
                    bits = LM_BITS;
                    break;
                case 2:
                    if(list[current/4] & RM_BITS)
                        break;
                    if(verbose==true && ~(list[current/4] & RM_BITS))
                        fprintf(stderr, "\tmarked %ld\n", current);
                    bits = RM_BITS;
                    break;
                case 3:
                    if(list[current/4] & RR_BITS)
                        break;
                    if(verbose==true)
                        fprintf(stderr, "\tmarked %ld\n", current);
                    bits = RR_BITS;
                    break;
            }
            list[current/4] |= bits;
            composites[compositescount]=current;

            ++count;
        }
        
        while(is_set==true)
        {
            ++currentprime;
            
            switch ( currentprime % 4 )  //Checks bits to find next prime
            {
                case 0:
                    is_set = list[currentprime/4] & LL_BITS;
                    break;
                case 1:
                    is_set = list[currentprime/4] & LM_BITS;
                    break;
                case 2:
                    is_set = list[currentprime/4] & RM_BITS;
                    break;
                case 3:
                    is_set = list[currentprime/4] & RR_BITS;
                    break;
            }
        }
        primes[primecount]=currentprime;
        ++primecount;
        is_set=true;

    }

    while(currentprime<=bound)
    {
        while(is_set==true)
        {
            ++currentprime;
            
            switch ( currentprime % 4 )  //Checks bits to find next prime
            {
                case 0:
                    is_set = list[currentprime/4] & LL_BITS;
                    break;
                case 1:
                    is_set = list[currentprime/4] & LM_BITS;
                    break;
                case 2:
                    is_set = list[currentprime/4] & RM_BITS;
                    break;
                case 3:
                    is_set = list[currentprime/4] & RR_BITS;
                    break;
            }
        }
        primes[primecount]=currentprime;
        ++primecount;
        is_set=true;
    }
    current=2;
    currentprime=2;
    switch(choice)
    {
        case 'p':
            // while(currentprime<=bound)
            // {
            //     while(is_set==true && currentprime<=bound)
            //     {                
            //         switch ( currentprime % 4 )  //Checks bits to find next prime
            //         {
            //             case 0:
            //                 is_set = list[currentprime/4] & LL_BITS;
            //                 break;
            //             case 1:
            //                 is_set = list[currentprime/4] & LM_BITS;
            //                 break;
            //             case 2:
            //                 is_set = list[currentprime/4] & RM_BITS;
            //                 break;
            //             case 3:
            //                 is_set = list[currentprime/4] & RR_BITS;
            //                 break;
            //         }
            //         ++currentprime;
            //     }
            //     printf("%ld\n", currentprime);
            //     ++currentprime;
            //     is_set=true;
                
            // }
            while(i<primecount && primes[i]<=bound)
            {
                printf("%ld\n", primes[i]);
                ++i;
            }
            break;
        case 'c':
            break;
    }
    return;
}
