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
void find_primes(long long bound, int verbose, char choice, int binary);


int main(int argc, char * argv[])
{
    
    long long bound=100;
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
                printf("Usage: ./sieve [-hvu:pcb]\n");
                return EXIT_FAILURE;
        }
    }
    find_primes(bound, verbose, choice, binary);

    return EXIT_SUCCESS;
}

void find_primes(long long bound, int verbose, char choice, int binary)
{
    uint8_t *list=malloc(((bound/4)+100));
    long count=2;            
    long long current=2;          //current number being checked
    long currentprime=current;
    uint8_t bits = 0x00;    //for setting and checking bits
    bool is_set=true;       //for checking if bit is set
    unsigned long printprime=1;

    if(binary==1)                    //remove before submitting
        binary=1;
    if(verbose==1)      //verbose output
    {
        fprintf(stderr,"upper bound: %lld\nshow primes: %s\nshow binary: %s\ntop check  : %ld\n", bound, choice=='p' ? "true" : "false", binary==1 ? "true" : "false", (long)sqrt(bound));
    } 
    count=2;
    
    
    while(currentprime< sqrt(bound))
    {
        current=currentprime;
        count=2;
        if(verbose==true)       //verbose output
            fprintf(stderr, "marking multiples of %ld\n", currentprime);
        while((long long)current<=bound)
        {
            current=count*currentprime; //prime*n, to mark each composite

            bits = 0x00;
            
            switch ( current % 4 )  //Sets the bits to mark composite numbers
            {
                case 0:
                    // if(list[current/4] & LL_BITS)
                    //     break;
                    if(verbose==true && ~(list[current/4] & LL_BITS))
                        fprintf(stderr, "\tmarked %lld\n", current);
                    bits = LL_BITS;
                    break;
                case 1:
                    // if(list[current/4] & LM_BITS)
                    //     break;
                    if(verbose==true && ~(list[current/4] & LM_BITS))
                        fprintf(stderr, "\tmarked %lld\n", current);
                    bits = LM_BITS;
                    break;
                case 2:
                    // if(list[current/4] & RM_BITS)
                        // break;
                    if(verbose==true && ~(list[current/4] & RM_BITS))
                        fprintf(stderr, "\tmarked %lld\n", current);
                    bits = RM_BITS;
                    break;
                case 3:
                    // if(list[current/4] & RR_BITS)
                    //     break;
                    if(verbose==true && ~(list[current/4] & RR_BITS))
                        fprintf(stderr, "\tmarked %lld\n", current);
                    bits = RR_BITS;
                    break;
            }
                if((long long)current<=bound)
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
            printprime=1;
            is_set=true;
            if(binary==0)
                printf("%d\n", 2);
            else
            {
                printprime=2;
                write(STDOUT_FILENO,&printprime,sizeof(printprime));
                printprime=1;
            }
            while((long long)printprime<=bound)
            {
                while(is_set==true && (long long)printprime<=bound)
                {                
                    ++printprime;
                    ++printprime;
                    if((long long)printprime<=bound)
                    {
                        switch ( printprime % 4 )  //Checks bits to find next prime
                        {
                            case 0:
                                is_set = list[printprime/4] & LL_BITS;
                                break;
                            case 1:
                                is_set = list[printprime/4] & LM_BITS;
                                break;
                            case 2:
                                is_set = list[printprime/4] & RM_BITS;
                                break;
                            case 3:
                                is_set = list[printprime/4] & RR_BITS;
                                break;
                        }
                    }
                    
                }
                if(binary==1 && (long long)printprime<=bound)
                {
                    write(STDOUT_FILENO,&printprime,sizeof(unsigned long));
                }
                else if((long long)printprime<=bound)
                    printf("%ld\n", printprime);
                
                
                
                is_set=true;
                
            }

            break;
        case 'c':
            printprime=1;
            is_set=false;
            while((long long)printprime<=bound)
            {
                while(is_set==false && (long long)printprime<=bound)
                {                
                    ++printprime;
                    switch ( printprime % 4 )  //Checks bits to find next prime
                    {
                        case 0:
                            is_set = list[printprime/4] & LL_BITS;
                            break;
                        case 1:
                            is_set = list[printprime/4] & LM_BITS;
                            break;
                        case 2:
                            is_set = list[printprime/4] & RM_BITS;
                            break;
                        case 3:
                            is_set = list[printprime/4] & RR_BITS;
                            break;
                    }
                    
                }
                if(binary==1 && (long long)printprime<=bound)
                {
                    write(STDOUT_FILENO,&printprime,sizeof(printprime));
                }
                else if((long long)printprime<=bound)
                    printf("%ld\n", printprime);

                is_set=false;
                
            }
            break;
    }
    free(list);
    list=NULL;
    return;
}
