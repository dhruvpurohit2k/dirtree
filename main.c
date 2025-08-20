#include "Tree.h"
#include "string.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PATH_SEPERATOR "/"

int check_if_ingnored_file(struct dirent *, char **, int);

enum ARG_VALUE { ARG_PATH, ARG_IGNORE_FILES, ARG_DEPTH, ARG_PARSE_FINISHED };

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
      add_all_character(child_name, PATH_SEPERATOR);
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
  struct string *root_name = create_string();
  struct string *path = create_string();
  // Parsing the arguments

  int size_ignore_files = 0;
  int ignore_files_count = 2;
  char **ignore_files = (char **)malloc(sizeof(char *) * ignore_files_count);
  int depth_level = 1;
  enum ARG_VALUE current_arg = ARG_PATH;
  enum ARG_VALUE past_arg = current_arg;
  char *buffer = (char *)malloc(sizeof(char) * 256);
  getcwd(buffer, 256);
  add_all_character(root_name, buffer);
  add_all_character(path, root_name->c);
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      if (current_arg == ARG_PARSE_FINISHED && argv[i][0] != '-') {
        switch (past_arg) {
        case ARG_DEPTH:
          printf(" -d ONLY TAKES ONE ARG\n");
          break;
        case ARG_PATH:
          printf(" PATH OPTION TAKES ONLY TAKES ONE ARG\n");
          break;
        default:
          printf("MALFORM ARGUMNET\n");
          break;
        }
        return -1;
      }
      if (argv[i][0] == '-') {
        if (argv[i][1] == '\0') {
          printf("EMPTY OPTION PROVIDED. PLEASE USE -d TO DEFINE DEPTH -p FOR "
                 "PATH -i FOR INGNORE FILES\n");
        } else {
          char option = argv[i][1];
          switch (option) {
          case 'p':
            current_arg = ARG_PATH;
            break;
          case 'i':
            current_arg = ARG_IGNORE_FILES;
            break;
          case 'd':
            current_arg = ARG_DEPTH;
            break;
          default:
            printf("INVALID OPTION -%c\n", option);
            break;
          }
        }
        continue;
      }

      switch (current_arg) {
      case ARG_DEPTH:
        if (argv[i][1] != '\0') {
          printf("SUPPORTS ONLY NUMBERS BETWEEN 0 and 9. Defaulting to 1\n");
          depth_level = 1;
        } else {
          depth_level = argv[i][0] - '0';
        }
        past_arg = current_arg;
        current_arg = ARG_PARSE_FINISHED;
        break;
      case ARG_IGNORE_FILES:
        if (size_ignore_files > ignore_files_count - 1) {
          ignore_files_count *= 2;
          ignore_files = (char **)realloc(ignore_files,
                                          sizeof(char *) * ignore_files_count);
        }
        ignore_files[size_ignore_files++] = argv[i];
        break;
      case ARG_PATH:
        remove_string(root_name, 0, root_name->size);
        remove_string(path, 0, path->size);
        add_all_character(root_name, realpath(argv[i], NULL));
        add_all_character(path, root_name->c);
        past_arg = current_arg;
        current_arg = ARG_PARSE_FINISHED;
        break;
      default:
        continue;
      }
    }
  }

  size_t i = root_name->size - 2;
  // Getting the name of the top level directory
  while (i > 0 && root_name->c[i] != '/')
    i--;
  remove_string(root_name, 0, i + 1);

  // Creating the directory tree
  struct Tree *root = create_Tree(root_name);
  root =
      make_DirTree(path, root, ignore_files, size_ignore_files, 0, depth_level);

  // printing the tree
  print_tree(root, 0);

  // freeing the tree
  free_tree(root);
  free(ignore_files);
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
