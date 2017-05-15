// You can use these includes and defines as a hint what functions you might want to use ;)
#define _GNU_SOURCE
#include "md5.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define HASH_LENGTH 16

int main (){
	// unsigned char target[HASH_LENGTH] = {0xe6, 0x54, 0x93, 0xcc, 0xde, 0xe9, 0xc4, 0x51, 0x4f, 0xe2, 0x0e, 0x04, 0x04, 0xf3, 0xbc, 0xb8}; "???" - to be used for les_miserables.txt
	unsigned char target[HASH_LENGTH] = {0x1c, 0x01, 0x43, 0xc6, 0xac, 0x75, 0x05, 0xc8, 0x86, 0x6d, 0x10, 0x27, 0x0a, 0x48, 0x0d, 0xec}; // "Les Miserables" - to be used for les_miserables_preface.txt
	time_t start = time(NULL);
	FILE* f;
	char c;

	f = fopen("les_miserables_preface.txt", "r");
	if(f == NULL){
		return 1;
	}
	/**while((c=fgetc(f))!=EOF){
	        printf("%c",c);
				}**/

	const int bsize=4096;
	char Buffer[bsize];
	char *Buffer_Ptr=Buffer;
	const char* delim = " ";

	while( !feof(f) )
	{
					Buffer_Ptr=fgets(Buffer_Ptr,bsize,f);
					if( Buffer_Ptr != NULL )
					{
									printf("%s",Buffer_Ptr);   // would printing the complete string read from input file
									char *Token=NULL; /* Token delimited by ";" from Buffer_Ptr */
									int Token_Cnt=0; /* Token Counter */
									Token=strtok(Buffer_Ptr,delim);
									while( Token != NULL){
										printf("%-2d %s  ",Token_Cnt++,Token);
										Token=strtok(NULL,delim);
									}
									printf("\n");
									
								}
			}

	/**char *ptr;

	ptr = strtok(string, delim);

	while(ptr != NULL) {
		printf("Abschnitt gefunden: %s\n", ptr);
		// naechsten Abschnitt erstellen
	 	ptr = strtok(NULL, delim);
	}**/
	/*
	 * - find all unique words from the book
	 * - create the hashes of all two-string combinations
	 * - try to speed it up
	 * Usage example (not complete!):
	 *
	 * unsigned char hash[HASH_LENGTH] = {0};
	 * char buffer[1024];
	 * // put something into buffer to hash it
	 * MD5_CTX md5_ctx;
	 * MD5_Init(&md5_ctx);
	 * MD5_Update(&md5_ctx, buffer, strlen(buffer));
	 * MD5_Final(hash, &md5_ctx);
	 */

	time_t end = time(NULL);
	printf("Execution took ~%fs\n", difftime(end, start));
	return 0;
}
