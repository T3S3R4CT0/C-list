#include "stddef.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
typedef struct {
     void* items;
     size_t length; size_t capacity;
     size_t type_size;
} List;

List create_list(size_t capacity, size_t type_size, bool* err);
List create_list_from_buffer(size_t capacity, size_t type_size, void* buffer);
void destroy_list(List* self);
void list_append_no_resize(List* self, void* value);
void list_append(List* self, void* value, bool* err);
void list_insert_no_resize(List* self, void* value, int index);
void list_remove(List* self, uint32_t index);
void list_remove_ordered(List* self, uint32_t index);

#ifdef USING_LIST

List create_list(size_t capacity, size_t type_size, bool* err) {
     auto buffer = malloc(capacity * type_size); if (!buffer) {
          *err = true;
          return (List) {};
     }

     *err = false;
     return (List) {
          .items = buffer,
          .length = 0,
          .capacity = capacity,
          .type_size = type_size
     };
}

List create_list_from_buffer(size_t capacity, size_t type_size, void* buffer) {
     return (List) {
          .items = buffer,
          .length = 0,
          .capacity = capacity,
          .type_size = type_size
     };
}

void destroy_list(List* self) {
     free(self->items);
}

void list_append_no_resize(List* self, void* value) {
     if (self->length >= self->capacity) abort();
     memcpy(self->items + self->type_size * self->length++, value, self->type_size);
}

void list_append(List* self, void* value, bool* err) {
     if (self->length >= self->capacity) {
          size_t new_capacity = self->capacity * 2;
          void* new_buffer = malloc(new_capacity * self->type_size);
          memcpy(new_buffer, self->items, self->length * self->type_size);

          free(self->items);
          self->items = new_buffer;
     }

     memcpy(self->items + self->type_size * self->length++, value, self->type_size);
}

void list_insert_no_resize(List* self, void* value, int index) {
     if (self->length >= self->capacity ||
         index >= self->length) abort();

     for (int i = index; i < self->length; i++) {
          memcpy(self->items + i * self->type_size, self->items + (i + 1) * self->type_size, self->type_size);
     }

     memcpy(self->items + index * self->length, value, self->type_size);

}

void list_insert(List* self, void* value, int index, bool* err) {
     if (index >= self->length) {
          *err = true;
          return;
     }

     if (self->length >= self->capacity) {
          auto new_capacity = self->capacity * 2;
          auto new_buffer = malloc(new_capacity * self->type_size);
          memcpy(new_buffer, self->items, self->capacity * self->type_size);
          
          free(self->items);
          self->items = new_buffer;
     }

     for (int i = index; i < self->length; i++) {
          memcpy(self->items + i * self->type_size, self->items + (i + 1) * self->type_size, self->type_size);
     }

     memcpy(self->items + index * self->length, value, self->type_size);
     *err = false;
}

void list_remove(List* self, uint32_t index) {
     memcpy(self->items + self->type_size * index, self->items + (self->length - 1) * self->type_size, self->type_size);
     self->length--;
}

void list_remove_ordered(List* self, uint32_t index) {
     for (int i = index; i < self->length - 1; i++) {
          memcpy(self->items  + self->type_size * i, self->items + (i + 1) * self->type_size, self->type_size);
     }

     self->length--;
}

#endif
