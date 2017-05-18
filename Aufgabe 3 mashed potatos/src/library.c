// You can use these includes and defines as a hint what functions you might want to use ;)
//#define _GNU_SOURCE
#include "md5.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <pthread.h>

#define HASH_LENGTH 16
// unsigned char target[HASH_LENGTH] = {0xe6, 0x54, 0x93, 0xcc, 0xde, 0xe9, 0xc4, 0x51, 0x4f, 0xe2, 0x0e, 0x04, 0x04, 0xf3, 0xbc, 0xb8}; "???" - to be used for les_miserables.txt
unsigned char target[HASH_LENGTH] = {0x1c, 0x01, 0x43, 0xc6, 0xac, 0x75, 0x05, 0xc8, 0x86, 0x6d, 0x10, 0x27, 0x0a, 0x48, 0x0d, 0xec}; // "Les Miserables" - to be used for les_miserables_preface.txt

int wordFound = 0;
char *queue[3];

struct arg_struct {
    const char *file;
    int thread;
};


int countlines(const char *file){
	FILE* fp = fopen(file, "r");
	int ch, number_of_lines = 0;
  do
	{
    ch = fgetc(fp);
    if(ch == '\n')
        number_of_lines++;
	} while (ch != EOF);

	if(ch != '\n' && number_of_lines != 0)
    number_of_lines++;
	fclose(fp);
  return number_of_lines;
}

void makeFile(const char *file, const char *outputFile, int lineCounter, int start){
	FILE *fp = fopen(file,"r");
	FILE *out = fopen(outputFile,"a");
	char buffer[255];
	int i = 1;
	for(;i<=start;i++){
		fgets(buffer, 255, (FILE*)fp);
	}
	while (feof(fp)==0 && i<= lineCounter) {
		fgets(buffer, 255, (FILE*)fp);
		if(feof(out)==0){
			fputs(buffer,out);
			i++;
		}
	}
}

void searchWords(const char *file){
	char buffer[255];
	char wordbuffer[255];
	int notinlist=1;
  FILE *fp = fopen(file, "r");

	while (feof(fp)==0) {
		fgets(buffer, 255, (FILE*)fp);
		//printf("\n%s\n Not inList: %d \n", buffer, notinlist);
		FILE *wr = fopen("Words_unique.txt", "a+");
		//printf("%d\n", feof(wr)==0);
		while(feof(wr)==0 && notinlist){
			fgets(wordbuffer, 255, (FILE*)wr);
			if(strcmp(wordbuffer,buffer)== 0){
 				notinlist = 0;
			}
		}

		if(notinlist==1){
			fputs(buffer,wr);
		}
		notinlist=1;
		fclose(wr);
	}
	fclose(fp);
}

void delimWords(const char *file, const char *delimeter){
	char buffer[255];
	FILE *fp = fopen(file, "r");
	FILE *wr = fopen("Words.txt", "a");
	char *ptr;
	//fp = fopen("les_miserables_preface", "r");
 while(feof(fp)==0){
		fscanf(fp, "%s", buffer);
		ptr=strtok(buffer, delimeter);

		while(ptr !=NULL){
			fputs(ptr,wr);
			fputs("\n",wr);
			ptr=strtok(NULL,delimeter);
  	}
  }
	fclose(fp);
	fclose(wr);
	searchWords("Words.txt");
}
int compare(unsigned char *a, unsigned char *b, int size) {

	    while(size-- > 0) {

	        if ( *a != *b ) { return (*a < *b ) ? -1 : 1; }

	        a++; b++;

	    }

	    return 0; //equal

	}


void *hashIt(void *arguments){
  struct arg_struct *args = (struct arg_struct *)arguments;
	const char *file = args->file;
	int thread = args->thread;
	FILE *fp = fopen(file,"r");
  int stop = 0;
	char word1[255];
	char word2[255];
	char buffer[1024];
	unsigned char hash[HASH_LENGTH] = {0};
	while(feof(fp)==0 && wordFound == 0 && stop == 0){ //check if other thread did not work on the same word
		fgets(word1, 255, (FILE*)fp);
		queue[thread]=word1;
		for(int i=0;i<=3;i++){
			if(i!=thread){
				if(word1 == queue[i]){
					stop =1;
				}
			}
		}
		FILE *fp2 = fopen("Words_unique.txt", "r"); //prev. file
		while(feof(fp2)==0 && wordFound == 0 && stop == 0){
			fgets(word2, 255, (FILE*)fp);
			//concatinate Strings buffer = word1 + " "+word2
			strcpy(buffer, word1); //word1
			strcpy(buffer, " ");
			strcpy(buffer, word2); //word2

			MD5_CTX md5_ctx;
			MD5_Init(&md5_ctx);
			MD5_Update(&md5_ctx, buffer, strlen(buffer));
			MD5_Final(hash, &md5_ctx);
    //  for(int i=0;i<HASH_LENGTH;i++)
      //  printf("%x\n",hash[i]);

      int x,y;
      for (x = 1; x<sizeof(hash);x++){
        printf("%x", hash[x]);
      }
      printf("\n");
      for (y = 1; y<sizeof(target);y++){
        printf("%x", target[y]);
      }
      printf("\n");

		 	if(compare(hash, target, HASH_LENGTH) == 0){
				wordFound=1;
				printf("\n%s\n", buffer);
      }

	  }
	fclose(fp2);

 }
 fclose(fp);

return NULL;
}

int main (){

	time_t start = time(NULL);
	//FILE* f;

	//f = fopen("/home/sven/Dokumente/Aufgabe 3/les_miserables.txt", "r");
	//if(f == NULL){
		//return 1;
	//}
   /* unsigned char target_dec[HASH_LENGTH];
  unsigned char *temp=0;
  for(int i=0;i<HASH_LENGTH;i++){
    temp = &target[i];
    target_dec = strtol(temp,NULL, 0);
  }*/
	const char* delim = " .,;-:0123456789?!\"*+()|&[]#$/%%'";
	//Find all unique Words in les_miserables.txt
  delimWords("les_miserables_preface.txt",delim);
 //count all Lines/Words of the Outputfile from delim Words
  int countLines = countlines("Words_unique.txt");
 // Split up the files so each thread can work on one File
  makeFile("Words_unique.txt","Thread1.txt",countLines/2, 1);
  makeFile("Words_unique.txt","Thread2.txt",countLines, countLines/2);
  system("tac Words_unique.txt > Words_reverse.txt");
  makeFile("Words_reverse.txt","Thread3.txt", countLines/2, 1);
  makeFile("Words_reverse.txt","Thread4.txt", countLines, countLines/2);


  /*
  *hash Test failed
  *unsigned char hash[HASH_LENGTH] = {0};
  *char buffer[1024];
  *buffer="Les Miserables";
  *FILE *t=fopen("Test.txt","r");
  *fgets(buffer, 1024, (FILE*)t);
  *printf("%s", buffer);
  *MD5_CTX md5_ctx;
  *MD5_Init(&md5_ctx);
  *MD5_Update(&md5_ctx, buffer, strlen(buffer));
  *MD5_Final(hash, &md5_ctx);
  *FILE *writer = fopen("Hash.txt","a");
  *fprintf(writer,"%s", hash);
  */



 //start threads
 pthread_t p1, p2, p3, p4;
struct arg_struct arg1, arg2, arg3, arg4;
arg1.file = "Thread1.txt";
arg1.thread = 0;

arg2.file = "Thread2.txt";
arg2.thread = 1;

arg3.file = "Thread3.txt";
arg3.thread = 2;

arg4.file = "Thread4.txt";
arg4.thread = 3;

pthread_create(&p1, NULL, hashIt, &arg1);
pthread_create(&p2, NULL, hashIt, &arg2);
pthread_create(&p3, NULL, hashIt, &arg3);
pthread_create(&p4, NULL, hashIt, &arg4);

system("rm -f Thread1.txt Thread2.txt Thread3.txt Thread4.txt");






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
