#include "Tree.h"
#include "string.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PATH_SEPERATOR "/"

int check_if_ingnored_file(struct dirent *, char **, int);

struct Tree *make_DirTree(struct string *path, struct Tree *parent,
                          char **ignore_files, int ignore_files_count,
                          int current_depth, int MAX_DEPTH) {
  DIR *dr;
  struct dirent *en;
  dr = opendir(path->c);
  while ((en = readdir(dr)) != NULL) {
    if (en->d_name[0] == '.')
      continue;
    if (en->d_type == DT_DIR) {
      struct string *child_name = create_string();
      add_all_character(child_name, en->d_name);
      add_all_character(child_name, "/");
      struct Tree *child = create_Tree(child_name);
      if (!check_if_ingnored_file(en, ignore_files, ignore_files_count) &&
          current_depth < MAX_DEPTH) {
        struct string *recurse_path = create_string();
        add_all_character(recurse_path, path->c);
        add_all_character(recurse_path, PATH_SEPERATOR);
        add_all_character(recurse_path, en->d_name);

        child = make_DirTree(recurse_path, child, ignore_files,
                             ignore_files_count, current_depth + 1, MAX_DEPTH);
        free(recurse_path);
      }
      add_child_Tree(parent, child);
    } else {
      struct string *child_name = create_string();
      add_all_character(child_name, en->d_name);
      struct Tree *child = create_Tree(child_name);
      add_child_Tree(parent, child);
    }
  }
  closedir(dr);
  return parent;
}

int main(int argc, char *argv[]) {

  // -------------------------Getting PWD's name
  struct string *root_name = create_string();
  struct string *path = create_string();
  char **ignore_files = NULL;
  if (argc == 1) {
    char *buffer = (char *)malloc(sizeof(char) * 256);
    getcwd(buffer, 256);
    add_all_character(root_name, buffer);
    add_all_character(path, root_name->c);
  } else {
    add_all_character(root_name, argv[1]);
    add_all_character(path, root_name->c);
  }
  if (argc > 1) {
    ignore_files = (char **)malloc(sizeof(char *) * argc - 1);
    for (int arg = 2; arg <= argc; arg++) {
      ignore_files[arg - 2] = argv[arg];
    }
  }

  size_t i = root_name->size - 2;
  while (i > 0 && root_name->c[i] != '/')
    i--;
  remove_string(root_name, 0, i + 1);
  struct Tree *root = create_Tree(root_name);
  root = make_DirTree(path, root, ignore_files, argc - 2, 0, 2);
  print_tree(root, 0);
  free_tree(root);
  return 0;
}

int check_if_ingnored_file(struct dirent *dir, char **ignored_files,
                           int ingnored_files_count) {
  for (int i = 0; i < ingnored_files_count; i++) {
    if (strcmp(dir->d_name, ignored_files[i]) == 0) {
      return 1;
    }
  }
  return 0;
}
