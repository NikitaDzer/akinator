#ifndef   AKINATOR_TREE_H
#define   AKINATOR_TREE_H

#include "config.h"

struct TreeNodeData
{
   char *string;
};

struct TreeNode
{
   TreeNode *daddy;
   TreeNode *yeapSon;
   TreeNode *nopeSon;
   
   TreeNodeData data;
};

struct Tree
{
   size_t    capacity;
   size_t    size;
   TreeNode *root;
   TreeNode *free;
   TreeNode *final;
};

const tree_error_t TREE_NO_ERROR =  0;
const tree_error_t TREE_ERROR    = -1;

const TreeNodeData TREE_NODE_DEFAULT_DATA = {
                   .string      = nullptr,
};

tree_error_t
tree_construct(Tree *const p_tree, const size_t capacity = TREE_NODES_DEFAULT_NUMBER);

void
tree_destruct(Tree *const p_tree);

tree_error_t
tree_fill_node(TreeNode *const p_node, const TreeNodeData *const p_data);

TreeNode*
tree_add_node(Tree *const p_tree, const TreeNodeData *const p_data = &TREE_NODE_DEFAULT_DATA);

tree_error_t
tree_relate_nodes(TreeNode *const p_daddy, TreeNode **const p_daddy_son, TreeNode *const p_son);

#endif // AKINATOR_TREE_H
