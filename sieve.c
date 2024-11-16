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
                if(isdigit(strtol(optarg, NULL, 10))!=0)
                {
                    return EXIT_FAILURE;
                }
                bound=strtol(optarg, NULL, 10);
                break;
            case 'b':
                binary=1;
                break;
            case 'v':
                verbose=1;
                break;
            case 'h':
                printf("  -u #  : Specify the upper bound for prime/composite numbers.\n          The default upper bound is 100.\n  -p    : Print the prime numbers (the default)\n  -c    : Print the composite numbers, instead of prime numbers.\n  -b    : Output the values as raw binary, not as ASCII text\n  -v    : Print some diagnostic messages to stderr\n  -h    : Print some amazing help messages and exit.\n");
                return EXIT_SUCCESS;
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
    long count=2;            
    long current=2;          //current number being checked
    long currentprime=current;
    uint8_t bits = 0x00;    //for setting and checking bits
    bool is_set=true;       //for checking if bit is set

    if(binary==1)                    //remove before submitting
        binary=1;
    if(verbose==1)      //verbose output
    {
        fprintf(stderr,"upper bound: %ld\nshow primes: %s\nshow binary: %s\ntop check  : %ld\n", bound, choice=='p' ? "true" : "false", binary==1 ? "true" : "false", (long)sqrt(bound));
    } 
    count=2;
    
    
    while(currentprime< sqrt(bound))
    {
        current=currentprime;
        count=2;
        if(verbose==true)       //verbose output
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
                    if(verbose==true && ~(list[current/4] & RR_BITS))
                        fprintf(stderr, "\tmarked %ld\n", current);
                    bits = RR_BITS;
                    break;
            }
            list[current/4] |= bits;

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
        is_set=true;

    }
    currentprime=1;
    is_set=true;
    
    switch(choice)
    {
        case 'p':
            currentprime=1;
            is_set=true;
            if(binary==0)
                printf("%d\n", 2);
            while(currentprime<=bound)
            {
                while(is_set==true && currentprime<=bound)
                {                
                    ++currentprime;
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
                if(binary==1 && currentprime<=bound)
                {
                    write(STDOUT_FILENO,(unsigned long *)&currentprime,1);
                }
                else if(currentprime<=bound)
                    printf("%ld\n", currentprime);
                
                
                
                is_set=true;
                
            }

            break;
        case 'c':
            currentprime=1;
            is_set=false;
            while(currentprime<=bound)
            {
                while(is_set==false && currentprime<=bound)
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
                if(binary==1 && currentprime<=bound)
                {
                    write(STDOUT_FILENO,(unsigned long *)&currentprime,1);
                }
                else if(currentprime<=bound)
                    printf("%ld\n", currentprime);

                is_set=false;
                
            }
            break;
    }
    return;
}
