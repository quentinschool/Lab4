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
    int size=sizeof(unsigned long);
    int op_fd;
    int amount=1;
    int i=0;
    unsigned long buffer[1];    
    do{
       
        op_fd=STDIN_FILENO;

        if(argc>=2)
        {
          op_fd=open(argv[i+1],O_RDONLY);
        }
        amount=read(op_fd,buffer,size);
        while(amount!=0)
        {
            if(amount!=0)  
            {

                printf("%lu",buffer[0]);

            }   
            printf("\n");
            amount=read(op_fd,buffer,size);

        }
        close(op_fd);
        i++;
    }while(i<argc-1);

    return EXIT_SUCCESS;
}