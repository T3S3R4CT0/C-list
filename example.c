#define USING_LIST

#include "stdio.h"
#include "list.h"

int main() {
     bool error;

     auto numbers = createList(100, sizeof(float), &error); if (error) {
          fprintf(stderr, "error at creating number list\n");
          return 1;
     }
     float floatValue = 0;

     floatValue = 6; list_append(&numbers, &floatValue, &error); if (error) {
          fprintf(stderr, "Error at appending 6 to list\n");
          return 1;
     }
     floatValue = 7; list_append(&numbers, &floatValue, &error); if (error) {
          fprintf(stderr, "Error at appending 7 to list\n");
          return 1;
     }

     for (int i = 0; i < numbers.length; i++) {
          printf("numbers[%d] = %f\n", i, ((float*)numbers.items)[i]);
     }

     
     destroyList(&numbers);
     return 0;
}
