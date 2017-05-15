#include "md5.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
//#include <md5.h>

#define HASH_LENGTH 16

//#include <openssl/sha.h>

/* user have to free the space returned by pointer of the function */
unsigned char *MD5(const char *string)
{
        unsigned char hash[HASH_LENGTH];
        char *MD5_start_ptr=malloc(65);
        char *MD5_ptr=MD5_start_ptr; /* save start address */
        MD5_CTX MD5;
        MD5_Init(&MD5);
        MD5_Update(&MD5,string,strlen(string));
        MD5_Final(hash, &MD5);
        int i=0;
        int x=0; /* count for the sha256 hex string length */
        bzero(MD5_ptr,65);
        for(i=0; i < HASH_LENGTH; i++)
        {
                snprintf(MD5_ptr,3,"%02x", hash[i]); /* write alway 2 bytes plus '0' */
                MD5_ptr+=2; /* next pointer only 2 bytes forward and overwrite last '0' */
                if(x > 64)
                {
                        fprintf(stderr,"%-3d : ERROR: sha256(): length of 64 exceeded\n",__LINE__);
                }
        }
        return((unsigned char*)MD5_start_ptr);
}

/* covers to change easy the hash algorithm here */
unsigned char *hash(const char *str)
{
        return(MD5(str));
}

int main(int argc, char **argv)
{
        int rc=0;
        const char *filename=NULL;

        if( argv[1] != NULL ) /* argument 1 should be the file name */
        {
                filename=argv[1];
                FILE *fp;
                fp=fopen(filename,"r");
                if( fp != NULL )
                {
                        const int bsize=4096;
                        char Buffer[bsize];
                        char *Buffer_Ptr=Buffer;
                        const char* delim=";";  /* list of delimiters  */
                        while( !feof(fp) )
                        {
                                Buffer_Ptr=fgets(Buffer_Ptr,bsize,fp);
                                if( Buffer_Ptr != NULL )
                                {
                                        /* printf("%s",Buffer_Ptr);   <-- would printing the complete string read from input file ! */
                                        char *Token=NULL; /* Token delimited by ";" from Buffer_Ptr */
                                        int Token_Cnt=0; /* Token Counter */
                                        Token=strtok(Buffer_Ptr,delim);     /* Token will hold the first part delimited by the delimiter given in variable "delim" */
                                        if( Token != NULL )
                                        {
                                                printf("%-2d %s\n",Token_Cnt++,Token);
                                                while( (Token=strtok(NULL,delim)) != NULL ) /* print rest of the string delimited by ";" */
                                                {
                                                        unsigned char *hash_ptr=hash(Token);
                                                        printf("%-2d %s   hash=[%s]\n",Token_Cnt++,Token,hash_ptr);
                                                        free(hash_ptr);
                                                }
                                        }
                                        else /* well never see that this happend bit anyway - error handling in case we got NULL in the first call of strtok() here: */
                                        {
                                                fprintf(stderr,"%-3d : WARNING: The  strtok()  function is unable to break the string into a sequence of zero or more nonempty tokens: %s\n",__LINE__,Buffer_Ptr);
                                        }
                                        /* good practise to clean buffer with zero for the next read: */
                                        bzero(Buffer_Ptr,bsize);
                                }
                        }
                        fclose(fp);
                }
                else
                {
                        fprintf(stderr,"ERROR: can not open file %s : %s\n",filename,strerror(errno));
                }
        }
        else
        {
                fprintf(stderr,"ERROR: argument 1 should be the name of the input file\n");
                rc=1;
        }

        return(rc);
}
