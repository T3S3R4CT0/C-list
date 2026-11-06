#define USING_LIST

#include "stdio.h"
#include "list.h"

int main() {
     bool err;

     auto list = create_list(10, sizeof(int), &err);     
     int int_value;

     int_value = 6; list_append(&list, &int_value, &err);
     int_value = 7; list_append(&list, &int_value, &err);
     int_value = 6; list_append(&list, &int_value, &err);
     int_value = 9; list_append(&list, &int_value, &err);

     int_value = 0; list_insert_no_resize(&list, &int_value, 3);

     //list_remove(&list, 0);


     for (int i = 0; i < list.length; i++) {
          auto current_value = ((int*)list.items)[i];
          printf("%d\n", current_value);
     }

     return 0;
}
