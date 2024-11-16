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
    int op_fd;
    int amount=1;
    unsigned long buffer[1];
    
    for( int i=1;i<argc;i++)
    {
        if(argc<3)
            op_fd=STDIN_FILENO;
        else
          op_fd=open(argv[i],O_RDONLY);
        amount=read(op_fd,buffer,4);
        while(amount!=0)
        {
            if(amount!=0)  
                printf("%lu",buffer[1]);
                
            printf("\n");
            amount=read(op_fd,buffer,4);

        }
        close(op_fd);
    }

    return EXIT_SUCCESS;
}