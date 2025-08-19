#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#ifndef CUSTOM_STRING_H
#define CUSTOM_STRING_H

struct string {
  size_t size;
  size_t capacity;
  char *c;
};

struct string *create_string() {
  struct string *new_string = (struct string *)malloc(sizeof(struct string));
  new_string->size = 0;
  new_string->capacity = 256;
  new_string->c = (char *)malloc(sizeof(char) * new_string->capacity);

  return new_string;
}

void print_string(struct string *string) { printf("%s\n", string->c); }

int append_string(struct string *string, char character) {
  if (string->size >= string->capacity - 2) {
    string->capacity = string->capacity * 2;
    string->c = (char *)realloc(string->c, sizeof(char) * string->capacity);
  }

  string->c[string->size++] = character;
  string->c[string->size] = '\0';

  return 0;
}

int remove_string(struct string *string, int start, int end) {
  if (start > end)
    return -1;
  int temp = start;
  for (int i = end; i <= string->size; i++) {
    string->c[temp++] = string->c[i];
  }
  string->size -= (end - start);
  return 0;
}

int add_all_character(struct string *string, char *characters) {
  while (*characters) {
    append_string(string, *characters);
    characters++;
  }

  return 0;
}

int free_string(struct string *string) {
  free(string->c);
  free(string);
  return 0;
}

#endif
