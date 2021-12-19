#ifndef   AKINATOR_DATABASE_H
#define   AKINATOR_DATABASE_H

#include "config.h"
#include "tree.h"

const char DATABASE_DEFAULT_PATH[64] = "database.txt";

void
tree_to_database(const char *const database_path, Tree *const p_tree);

void
database_to_tree(const char *const database_path, Tree *const p_tree);

#endif // AKINATOR_DATABASE_H
