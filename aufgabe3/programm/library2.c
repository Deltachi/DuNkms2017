
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
        int rc=0;
        const char *filename=NULL;

        if( "les_miserables_preface.txt" != NULL )
        {
                filename=argv[1];
                FILE *fp;
                fp=fopen("les_miserables_preface.txt","r");
                if( fp != NULL )
                {
                        const int bsize=4096;
                        char Buffer[bsize];
                        char *Buffer_Ptr=Buffer;
                        const char* delim=" .,;-:0123456789?!\"*+()|&[]#$/%%'";  // list of delimiters
                        while( !feof(fp) )
                        {
                                Buffer_Ptr=fgets(Buffer_Ptr,bsize,fp);
                                if( Buffer_Ptr != NULL )
                                {
                                        printf("%s",Buffer_Ptr);   // would printing the complete string read from input file
                                        char *Token=NULL; /* Token delimited by ";" from Buffer_Ptr */
                                        int Token_Cnt=0; /* Token Counter */
                                        Token=strtok(Buffer_Ptr,delim);     /* Token will hold the first part delimited by the delimiter given in variable "delim" */
                                        if( Token != NULL )
                                        {
                                                printf("%-2d %s\n",Token_Cnt++,Token);
                                                //while( (Token=strtok(NULL,delim)) != NULL ) /* print rest of the string delimited by ";" */
                                                //{
                                                //        unsigned char *hash_ptr=hash(Token);
                                                //        printf("%-2d %s   hash=[%s]\n",Token_Cnt++,Token,hash_ptr);
                                                //        free(hash_ptr);
                                                //}
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
