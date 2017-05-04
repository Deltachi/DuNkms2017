#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//linked list element or cells that are inside one row
struct cell_t {
   int contents;
   struct cell_t* next_element;
};

//helper struct to create a linked list of pointers to the first_elements of each row
struct row_t {
   struct cell_t* first_element;
   struct row_t* next_row;
};

//recursive function that creates a linked list(of cell_t elements)
//the initial function call returns a pointer to the first element of that list
struct cell_t* build_row(int number_elements){
   struct cell_t* a;
   a = malloc(sizeof(struct cell_t));
   a->contents = rand() % 100;
   if (number_elements > 1) {
      a->next_element = build_row(--number_elements);
   }else{
      a->next_element = NULL;
   }
   return a; //returns pointer to the active element
}

//recursive function that creates a linked list of tupels(first_element of row pointer, next_row_pointer)
//the initial function call returns a pointer to the first element of that list
struct row_t* grid_init(int number_elements, int number_rows){
   struct row_t* r;
   r = malloc(sizeof(struct row_t));
   r->first_element = build_row(number_elements);
   if (number_rows > 1) {
      r->next_row = grid_init(number_elements,--number_rows);
   }else{
      r->next_row = NULL;
   }
   return r;
}

void grid_print_dot(struct row_t* row_pointer){
   struct cell_t* p;       //cell pointer
   struct row_t* row_p;    //row_loop_pointer
   row_p = row_pointer;
   //row while loop
   printf("digraph {\n");
   while (row_p != NULL) {
      printf("\t-row helper-\n");
      printf("\tM%p -> \tM0x%p;\n", row_p, row_p->first_element);
      printf("\tM%p -> ", row_p);
      if (row_p->next_row != NULL){
         printf("\tM%p;\n", row_p->next_row);
      }else{
         printf("\tNULL;\n");
      }
      p = row_p->first_element;
      //element of row while loop
      printf("\t-row contents-\n");
      while (p != NULL) {
         printf("\tM%p -> ", p);
         if (p->next_element != NULL){
            printf("\tM%p;\n", p->next_element);
         }else{
            printf("\tNULL;\n\n");
         }
         p = p->next_element;
      }
      row_p = row_p->next_row;
   }
   printf("}");
}
void grid_print(struct row_t* row_pointer){
   struct cell_t* cell_p;  //cell pointer
   struct row_t* row_p;    //row_loop_pointer
   row_p = row_pointer;
   //row while loop
   while (row_p != NULL) {
      cell_p = row_p->first_element;
      printf("|\t");
      //element of row while loop
      while (cell_p != NULL) {
         printf("%d\t",cell_p->contents);
         cell_p = cell_p->next_element;
      }
      printf("|\n");
      row_p = row_p->next_row;
   }
}
int main(int argc, char const *argv[]) {
   struct row_t* row_c;    //rows_first_row_pointer

   srand(time(NULL));      //initialisiere Zufallsvariable
   row_c = grid_init(4,7); //Aufgabenteil 1
   grid_print(row_c);      //Aufgabenteil 2
   grid_print_dot(row_c);  //Aufgabenteil 3

   return 0;
}
