//
// Created by User on 25.11.2021.
//

#include "../include/tree.h"

typedef tree_error_t error_t;

static size_t N_ADD_NODES = 16;

error_t
tree_construct(Tree *const p_tree, const size_t capacity)
{
   #ifdef    TREE_ANTI_FOOL
   if (p_tree == nullptr)
      return TREE_ERROR;
   
   if (capacity > TREE_MAX_CAPACITY)
      return TREE_ERROR;
   #endif // TREE_ANTI_FOOL
   
   TreeNode *const nodes = (TreeNode *)calloc(capacity, sizeof(TreeNode));
   
   if (nodes == nullptr)
      return TREE_ERROR;
   
   p_tree->capacity = capacity;
   p_tree->size     = 0;
   p_tree->root     = nodes;
   p_tree->final    = nodes + capacity - 1;
   p_tree->free     = nodes;
   
   return TREE_NO_ERROR;
}

void
tree_destruct(Tree *const p_tree)
{
   //??????????????????????
   #ifdef    TREE_ANTI_FOOL
   if (p_tree != nullptr)
   {
      free(p_tree->root);
   
      p_tree->capacity = 0;
      p_tree->size  = 0;
      p_tree->root    = nullptr;
   }
   #else //  !TREE_ANTI_FOOL
   free(p_tree->root);
   
   p_tree->capacity = 0;
   p_tree->size  = 0;
   p_tree->root    = nullptr;
   #endif // !TREE_ANTI_FOOL
}

error_t
tree_fill_node(TreeNode *const p_node, const TreeNodeData *const p_data)
{
   #ifdef    TREE_ANTI_FOOL
   if (p_node == nullptr || p_data == nullptr)
      return TREE_ERROR;
   #endif // TREE_ANTI_FOOL
   
   p_node->data = *p_data;
   
   return TREE_NO_ERROR;
}

TreeNode*
tree_add_node(Tree *const p_tree, const TreeNodeData *const p_data)
{
   #ifdef    TREE_ANTI_FOOL
   if (p_tree == nullptr || p_data == nullptr)
      return nullptr;
   #endif // TREE_ANTI_FOOL
   
   if (p_tree->free + 1 == p_tree->final)
   {
      if (p_tree->capacity + N_ADD_NODES > TREE_MAX_CAPACITY)
         return nullptr;
      
      p_tree->final->daddy = (TreeNode *)calloc(N_ADD_NODES, sizeof(TreeNode));
      assert(p_tree->final->daddy);
      
      p_tree->capacity += N_ADD_NODES;
      p_tree->free      = p_tree->final->daddy;
      p_tree->final     = p_tree->final->daddy + N_ADD_NODES - 1;
   }
   
   tree_fill_node(p_tree->free, p_data);
   p_tree->size += 1;
   p_tree->free += 1;
   
   return p_tree->free - 1;
}

error_t
tree_relate_nodes(TreeNode *const p_daddy, TreeNode **const p_daddy_son, TreeNode *const p_son)
{
   #ifdef    TREE_ANTI_FOOL
   if (p_daddy == nullptr || p_daddy_son == nullptr || p_son == nullptr)
      return TREE_ERROR;
   #endif // TREE_ANTI_FOOL
   
  *p_daddy_son  = p_son;
   p_son->daddy = p_daddy;
   
   return TREE_NO_ERROR;
}

error_t
tree_visit(Tree *const p_tree)
{
   #ifdef    TREE_ANTI_FOOL
   if (p_tree == nullptr)
      return TREE_ERROR;
   #endif // TREE_ANTI_FOOL
   
   return TREE_NO_ERROR;
}