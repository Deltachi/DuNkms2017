#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int load_data(int** mat_max,int** mat_allocated,int* vec_all_res)
{
  int i,j;
  //open max matrix
  FILE *file;
  file=fopen("max.txt", "r");

  printf("\nMax Allocation Matrix:\n");
  for(i = 0; i < 5; i++)
  {
      for(j = 0; j < 5; j++)
      {
       if (!fscanf(file, " %d", &mat_max[i][j]))
          break;
       printf("%d\t",mat_max[i][j]);
      }
      printf("\n\n");
  }
  fclose(file);

  //open allocation matrix
  file=fopen("allocated.txt", "r");

  printf("\nCurrent Allocation Matrix:\n");
  for(i = 0; i < 5; i++)
  {
      for(j = 0; j < 5; j++)
      {
       if (!fscanf(file, " %d", &mat_allocated[i][j]))
          break;
       printf("%d\t",mat_allocated[i][j]);
      }
      printf("\n\n");
  }
  fclose(file);

  //open ressource vector
  file=fopen("ressources.txt", "r");

  printf("\nAll Ressources Vector:\n");
  for(j = 0; j < 5; j++)
  {
   if (!fscanf(file, " %d", &vec_all_res[j]))
      break;
   printf("%d\t",vec_all_res[j]);
  }
  printf("\n\n");
  fclose(file);

  return 0;
}
int calc_free_res(int**mat_allocated, int* vec_all_res, int* vec_free_res)
{
  printf("\nCalculate Free Ressources Vector...\n");
  for (int res_i = 0; res_i < 5; res_i++) {
    vec_free_res[res_i] = vec_all_res[res_i];
      for (int process_x = 0; process_x < 5; process_x++) {
          vec_free_res[res_i] -= mat_allocated[process_x][res_i];
      }
      printf("%d\t", vec_free_res[res_i]);
  }
  printf("\n");
  return 0;
}
int calc_mat_need(int** mat_max, int** mat_allocated, int** mat_need)
{
  printf("\nCalculate Need Matrix...\n");
  for (int process = 0; process < 5; process++) {
    for (int ressource = 0; ressource < 5; ressource++) {
      mat_need[process][ressource] = mat_max[process][ressource] - mat_allocated[process][ressource];
      printf("%d\t", mat_need[process][ressource]);
    }
    printf("\n\n");
  }
  return 0;
}
int get_next_process(int** mat_need, int* vec_free_res, int* finished)
{
  for (int process = 0; process < 5; process++) {
    for (int ressource = 0; ressource < 5; ressource++) {
      //if a ressource is not available; break
      if (vec_free_res[ressource] < mat_need[process][ressource] || finished[process])
      {
        break;
      }else
      {
        //if first to last ressource was available
        if (ressource == 4)
          return process;
      }
    }
  }
  return -1;
}
int update_free_res(int** mat_allocated, int* vec_free_res, int process, int* finished)
{
  printf("\nUPDATED Free Ressources Vector:\n");
  for (int ressource = 0; ressource < 5; ressource++) {
    vec_free_res[ressource] += mat_allocated[process][ressource];
    printf("%d\t", vec_free_res[ressource]);
  }
  printf("\n\n");
  finished[process] = 1;
  return 0;
}
int main()
{
  /*matrices*/
  int** mat_max=malloc(5*5*sizeof(int));
  int** mat_allocated=malloc(5*5*sizeof(int));
  int** mat_need=malloc(5*5*sizeof(int));
  int* vec_all_res=malloc(5*sizeof(int));
  int* vec_free_res=malloc(5*sizeof(int));
  int i;
  int j;
  int* finished=malloc(5*sizeof(int));
  for(i=0;i<5;++i)
  {
    mat_max[i]=malloc(5*sizeof(int));
    mat_allocated[i]=malloc(5*sizeof(int));
    mat_need[i]=malloc(5*sizeof(int));
    finished[i] = 0;
  }

  load_data(mat_max,mat_allocated,vec_all_res);
  calc_free_res(mat_allocated, vec_all_res, vec_free_res);
  calc_mat_need(mat_max, mat_allocated, mat_need);

  printf("\n-----------------\nStart algorithm..\n");
  int finished_processes = 0;
  int next_processID;
  do {
      next_processID = get_next_process(mat_need, vec_free_res, finished);
      if (next_processID == -1)
      {
        printf("\n=========\nDEADLOCK!\n=========\n");
      }else
      {
        printf("\n>>>>>>>>>\nProcess:%d\n>>>>>>>>>\n",next_processID);
        update_free_res(mat_allocated,vec_free_res,next_processID, finished);
        finished_processes++;
        printf("%d left..\n", 5 - finished_processes);
      }
  } while(next_processID != -1 && finished_processes < 5);
  if (finished_processes == 5) {
    printf("\n=$=$=$=$=\n!SUCCESS!\n=$=$=$=$=\n");
  }

  return 0;
}
