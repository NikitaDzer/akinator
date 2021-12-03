//
// Created by User on 25.11.2021.
//

#ifndef   AKINATOR_H
#define   AKINATOR_H

#include "config.h"
#include "tree.h"

const char   DATABASE_DEFAULT_PATH[64] = "database.txt";
const size_t AKINATOR_BUFFER_SIZE      = 1024;

tree_error_t
tree_to_database(const char *const database_path, Tree *const p_tree);

tree_error_t
database_to_tree(const char *const database_path, Tree *const p_tree);

tree_error_t
akinator(Tree *const p_tree);

#endif // AKINATOR_H
