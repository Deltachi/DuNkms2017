#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <sys/timeb.h>
#include <math.h>

int ROWS = 18000;
int COLS = 18000;
int TEST_RUNS = 10;

int runColWhise(int** arr) {
  struct timeb start, end;
  int diff;
  ftime(&start);
  int x,y;
  for (y = 0; y < ROWS; y++) {
    for (x = 0; x < COLS; x++) {
      arr[y][x] = arr[y][x]*1024;
    }
  }
  ftime(&end);
  diff = (int) (1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
  return diff;
}

int runRowWhise(int** arr) {
  struct timeb start, end;
  int diff;
  ftime(&start);
  int x,y;
  for (x = 0; x < COLS; x++) {
    for (y = 0; y < ROWS; y++) {
      arr[y][x] = arr[y][x]*1024;
    }
  }
  ftime(&end);
  diff = (int) (1000.0 * (end.time - start.time) + (end.millitm - start.millitm));
  return diff;
}


int main(int argc, char const *argv[]) {
  //initialization
  int** arr = malloc(ROWS*COLS*sizeof(int));
  int i,x;
  for (i = 0; i < ROWS; i++) {
    arr[i] = malloc(COLS*sizeof(int));
    for (x = 0; x < COLS; x++) {
      arr[i][x] = 10;
    }
  }
  //information
  printf("Sizeof int: %lu bytes\n", sizeof(int));
  printf("ROWS x COLS: [%d]x[%d]\n",ROWS,COLS);
  printf("Size of Array: %lu MBytes\n", ROWS*COLS*sizeof(int)/1000000);
  int time_spend;
  int time_collected;
  int time_spend_arr[TEST_RUNS];
  int time_variance;


  //TESTRUN
  //run collumn whise
  time_collected = 0;
  
  for (i = 0; i < TEST_RUNS; i++) {
    time_spend = runColWhise(arr);
    printf("COL-WHISE: \tExecution time %d/%d: \t%d milliseconds\n",i+1,TEST_RUNS,time_spend);
    time_collected += time_spend;
    time_spend_arr[i] = time_spend;
  }
  printf(">>> COL-EXEC: Average execution time: %d milliseconds.\n", (int)time_collected/TEST_RUNS);
  for (i = 0; i < TEST_RUNS; i++){
     time_variance += (time_spend_arr[i] - time_collected/TEST_RUNS) * (time_spend_arr[i] - time_collected/TEST_RUNS); //pow((time_spend_arr[i] - time_collected/TEST_RUNS),2.0);
  }
  time_variance = time_variance * 1/(TEST_RUNS);
  printf("Execution time variance: %d\n\n",time_variance);
  //run row whise
  time_collected = 0;
  for (i = 0; i < TEST_RUNS; i++) {
    time_spend = runRowWhise(arr);
    printf("ROW: \t\tExecution time %d/%d: \t%d milliseconds\n",i+1,TEST_RUNS,time_spend);
    time_collected += time_spend;
    time_spend_arr[i] = time_spend;
  }
  printf(">>> ROW-EXEC: Average execution time: %d milliseconds.\n", (int)time_collected/TEST_RUNS);
  for (i = 0; i < TEST_RUNS; i++){
     time_variance += (time_spend_arr[i] - time_collected/TEST_RUNS) * (time_spend_arr[i] - time_collected/TEST_RUNS); //pow((time_spend_arr[i] - time_collected/TEST_RUNS),2.0);
  }
  time_variance = time_variance * 1/(TEST_RUNS);
  printf("Execution time variance: %d\n\n",time_variance);

  return 0;
}
