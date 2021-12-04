#ifndef   AKINATOR_H
#define   AKINATOR_H

#include "config.h"
#include "tree.h"

const size_t AKINATOR_BUFFER_SIZE      = 1024;

tree_error_t
akinator(Tree *const p_tree);

#endif // AKINATOR_H
