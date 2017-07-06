#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <sys/timeb.h>

int ROWS = 6000;
int COLS = 6000;
int TEST_RUNS = 10;

int runColWhise(int** arr) {

  struct timeb start, end;
  int diff;
  ftime(&start);

  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
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

  for (int x = 0; x < COLS; x++) {
    for (int y = 0; y < ROWS; y++) {
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
  for (int i = 0; i < ROWS; i++) {
    arr[i] = malloc(COLS*sizeof(int));
    for (int x = 0; x < COLS; x++) {
      arr[i][x] = 10;
    }
  }
  //information
  printf("Sizeof int: %lu bytes\n", sizeof(int));
  printf("Size of Array: %lu MBytes\n", ROWS*COLS*sizeof(int)/1000000);
  int time_spend;
  int time_collected;

  //run row whise
  time_collected = 0;
  for (int i = 0; i < TEST_RUNS; i++) {
    time_spend = runRowWhise(arr);
    printf("ROW: \t\tExecution time %d/%d: \t%d milliseconds\n",i+1,TEST_RUNS,time_spend);
    time_collected += time_spend;
  }
  printf(">>> ROW: Average execution time: %d milliseconds.\n\n", (int)time_collected/TEST_RUNS);


  return 0;
}
