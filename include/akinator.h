//
// Created by User on 25.11.2021.
//

#ifndef   AKINATOR_H
#define   AKINATOR_H

#include "config.h"
#include "tree.h"

const char DATABASE_DEFAULT_PATH[64] = "database.txt";

tree_error_t
database_to_tree(const char *const database_path, Tree *const p_tree);

#endif // AKINATOR_H
