#include "string.h"
#include <stdlib.h>

#ifndef CUSTOM_TREE_H
#define CUSTOM_TREE_H

struct Tree {
  int childern_number;
  int MAX_CHILDRED;
  struct string *name;
  struct Tree **childern;
};

void indent(int);

struct Tree *create_Tree(struct string *name) {
  struct Tree *new_tree = (struct Tree *)malloc(sizeof(struct Tree));
  new_tree->MAX_CHILDRED = 5;
  new_tree->childern =
      (struct Tree **)malloc(sizeof(struct Tree *) * new_tree->MAX_CHILDRED);
  new_tree->childern_number = 0;
  new_tree->name = name;
  return new_tree;
}

void add_child_Tree(struct Tree *parent, struct Tree *child) {

  if (parent->childern_number >= parent->MAX_CHILDRED - 1) {
    parent->MAX_CHILDRED = parent->MAX_CHILDRED * 2;
    parent->childern = (struct Tree **)realloc(
        parent->childern, sizeof(struct Tree *) * parent->MAX_CHILDRED);
  }

  parent->childern[parent->childern_number++] = child;
}

int print_tree(struct Tree *tree, int indent_level) {
  print_string(tree->name);
  indent_level += tree->name->size / 2;
  for (int i = 0; i < tree->childern_number; i++) {
    indent(indent_level);
    printf("|\n");
    indent(indent_level);
    printf("|-> ");
    print_tree(tree->childern[i], indent_level + 3);
  }
  return 0;
}

void indent(int n) {
  while (n > 0) {
    putchar(' ');
    n--;
  }
}

void free_tree(struct Tree *tree) {
  if (tree->childern_number > 0) {
    for (int i = 0; i < tree->childern_number; i++)
      free_tree(tree->childern[i]);
  }
  free_string(tree->name);
  free(tree->childern);
  free(tree);
  return;
}

#endif
