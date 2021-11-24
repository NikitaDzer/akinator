//
// Created by User on 25.11.2021.
//

#ifndef   AKINATOR_TREE_H
#define   AKINATOR_TREE_H

#include "config.h"

struct TreeNode
{
   TreeNode *nopeSon;
   TreeNode *yeapSon;
   char     *string;
   size_t    string_size;
};

struct Tree
{
   size_t    n_nodes;
   TreeNode *nodes;
};


#endif // AKINATOR_TREE_H
