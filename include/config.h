#ifndef   AKINATOR_CONFIG_H
#define   AKINATOR_CONFIG_H

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>

#define TREE_ANTI_FOOL
#define SAY                               \
      printf("%ld -- HEAR!\n", __LINE__); \

typedef size_t tree_bitmask_t;

const char TREE_DUMP_PATH[64]    = "tree_dump.html";
const char TREE_DOTFILE_PATH[64] = "dotfile.dot";

const size_t TREE_MAX_CAPACITY     = SIZE_MAX >> 8;
const size_t TREE_INITIAL_CAPACITY = 64;

#endif // AKINATOR_CONFIG_H
