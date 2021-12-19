#ifndef   AKINATOR_TREE_H
#define   AKINATOR_TREE_H

#include "config.h"

enum TreeError
{
   PLUG              = 0 << 0,
   BAD_ALLOC         = 1 << 0,
   BAD_ARGUMENTS     = 1 << 1,
   
   BAD_PTR_TREE      = 1 << 4,
   BAD_PTR_NODE      = 1 << 5,
   BAD_PTR_DATA      = 1 << 6,
   BAD_PTR_RELATION  = 1 << 7,
   BAD_CAPACITY      = 1 << 8,
   BOUND_RELATIONS   = 1 << 9,
};

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

const TreeNodeData TREE_NODE_DEFAULT_DATA = {
         .string = nullptr,
};

Tree*
tree_construct(const size_t initialCapacity = TREE_INITIAL_CAPACITY, tree_bitmask_t *const p_bitmask = nullptr);

void
tree_destruct(Tree *const p_tree, tree_bitmask_t *const p_bitmask = nullptr);

TreeNode*
tree_add_node(Tree *const p_tree, const TreeNodeData *const p_data = &TREE_NODE_DEFAULT_DATA, tree_bitmask_t *const p_bitmask = nullptr);

TreeNode*
tree_relate(TreeNode *const p_daddy, TreeNode *const p_son, TreeNode **const p_relation, tree_bitmask_t *const p_bitmask = nullptr);

TreeNode*
tree_fill_node(TreeNode *const p_node, const TreeNodeData *const p_data, tree_bitmask_t *const p_bitmask = nullptr);

void
tree_parse_bitmask(const tree_bitmask_t bitmask);

#endif // AKINATOR_TREE_H
