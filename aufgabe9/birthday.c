#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int MAX_VALUE = 644;

char* getfield(char* line, int num)
{
    char* tok;
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}
int generateKey(char* date)
{
    int key = 0;
    int count = 100000;
    char* tok;
    for (tok = strtok(date, "/"); tok && *tok; tok = strtok(NULL, "/\n"))
    {
        for (int i = 0; i < 2; i++) {
          key += ((int)tok[i] - '0') * count;
          //printf("%c * %d = %d\n", tok[i],count, key);
          count = count/10;
        }
    }
    return key;
}

int main()
{
    int i;
    int j;
    int k;
    int l;
    int counter = 0;
    int iterations = 1000;
    int ergebnisse[365] = {0};

    time_t t;
    srand((unsigned) time(&t));
    int* list = malloc(MAX_VALUE*sizeof(int));
    int* list_partial = malloc(MAX_VALUE*sizeof(int));
    int index = 0;
    char line[1024];

    FILE* stream = fopen("kms_ex09_bundestagsabgeordnete.csv", "r");

    while (fgets(line, 1024, stream))
    {
        char* tmp = strdup(line);
        char* date = getfield(tmp, 2);
        int key;

        printf("%d: Date: %s\n",index, date);
        key = generateKey(date);
        printf("%d: Generated key: %d\n", index, key);
        list[index] = key;
        index++;
        free(tmp);
    }

    for (i = 0; i < iterations; i++){
        for (j = 1; j<= 365; j++)
        {
            for (k = 0; k <= j; k++)
            {
                //füge j zufällige Einträge in eine Liste zusammen
                list_partial[k] = list[rand() % MAX_VALUE];

            }
            //wenn liste größer als 1 ist
            if (j > 0){
                // vergleiche jedes element der liste mit der liste auf matches
                counter = 0;
                for (l = j-1; l >= 0; l--)
                {
                    if (list_partial[j] == list_partial[l])
                    {
                          counter++;
                          if (counter >= 2)
                          {
                            //printf("Ein Match für: k = %d\n", j);
                            ergebnisse[j]++;
                            break;
                          }
                    }
                }
            }
        }
    }
    for (i = 0; i < 365; i++)
    {
        int percent = ergebnisse[i]*100 / iterations;
        printf("Ergebnisse für k = %d \tund \t%d Durchläufe: \t%d Treffer -> \t%d%% \t", i, iterations, ergebnisse[i], percent);
        for (int perc = 0; perc < percent; perc++)
        {
          printf("\u2500\u2501");
        }
        printf("\n");
    }
}
