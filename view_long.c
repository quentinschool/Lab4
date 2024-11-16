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


int main(int argc,char * argv[])
{
    int arraysize=4;
    int op_fd;
    int amount=1;
    unsigned char buffer[arraysize];
    for( int i=1;i<argc;i++)
    {
        if(argc<2)
        {
            op_fd=STDIN_FILENO;
        }
        else
        {
          op_fd=open(argv[i],O_RDONLY);
        }
        amount=read(op_fd,buffer,arraysize);
        while(amount!=0)
        {
            if(amount!=0)  
            {
                for(int k=0;k<amount;k++)
                {
                    printf("%u",buffer[k]);
                }
            }   
            printf("\n");
            amount=read(op_fd,buffer,arraysize);

        }
        close(op_fd);
    }

    return EXIT_SUCCESS;
}