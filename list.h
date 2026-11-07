#include "stddef.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
typedef struct {
     void* items;
     size_t length; 
     size_t capacity;
     size_t typeSize;
} List;

List createList(size_t capacity, size_t type_size, bool* error);
List initListFromBuffer(size_t capacity, size_t typeSize, void* buffer);
void destroyList(List* self);
void list_append_no_resize(List* self, void* value);
void list_append(List* self, void* value, bool* error);
void list_insertNoResize(List* self, void* value, int index);
void list_insert(List* self, void* value, int index, bool* error);
void list_remove(List* self, uint32_t index);
void list_removeOrdered(List* self, uint32_t index);

#ifdef USING_LIST

List createList(size_t capacity, size_t typeSize, bool* error) {
     auto buffer = malloc(capacity * typeSize); if (!buffer) {
          *error = true;
          return (List) {};
     }

     *error = false;
     return (List) {
          .items = buffer,
          .length = 0,
          .capacity = capacity,
          .typeSize = typeSize
     };
}

List createListFromBuffer(size_t capacity, size_t typeSize, void* buffer) {
     return (List) {
          .items = buffer,
          .length = 0,
          .capacity = capacity,
          .typeSize = typeSize
     };
}

void destroyList(List* self) {
     free(self->items);
}

void list_appendNoResize(List* self, void* value) {
     if (self->length >= self->capacity) abort();
     memcpy(self->items + self->typeSize * self->length++, value, self->typeSize);
}

void list_append(List* self, void* value, bool* err) {
     if (self->length >= self->capacity) {
          size_t newCapacity = self->capacity * 2;
          void* newBuffer = malloc(newCapacity * self->typeSize);
          memcpy(newBuffer, self->items, self->length * self->typeSize);

          free(self->items);
          self->items = newBuffer;
     }

     memcpy(self->items + self->typeSize * self->length++, value, self->typeSize);
}

void list_insert_no_resize(List* self, void* value, int index) {
     if (self->length >= self->capacity ||
         index >= self->length) abort();

     for (int i = index; i < self->length; i++) {
          memcpy(self->items + i * self->typeSize, self->items + (i + 1) * self->typeSize, self->typeSize);
     }

     memcpy(self->items + index * self->length, value, self->typeSize);

}

void list_insert(List* self, void* value, int index, bool* error) {
     if (index >= self->length) {
          *error = true;
          return;
     }

     if (self->length >= self->capacity) {
          auto newCapacity = self->capacity * 2;
          auto newBuffer = malloc(newCapacity * self->typeSize);
          memcpy(newBuffer, self->items, self->capacity * self->typeSize);
          
          free(self->items);
          self->items = newBuffer;
     }

     for (int i = index; i < self->length; i++) {
          memcpy(self->items + i * self->typeSize, self->items + (i + 1) * self->typeSize, self->typeSize);
     }

     memcpy(self->items + index * self->length, value, self->typeSize);
     *error = false;
}

void list_remove(List* self, uint32_t index) {
     memcpy(self->items + self->typeSize * index, self->items + (self->length - 1) * self->typeSize, self->typeSize);
     self->length--;
}

void list_remove_ordered(List* self, uint32_t index) {
     for (int i = index; i < self->length - 1; i++) {
          memcpy(self->items  + self->typeSize * i, self->items + (i + 1) * self->typeSize, self->typeSize);
     }

     self->length--;
}

#endif
