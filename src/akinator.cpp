//
// Created by User on 25.11.2021.
//

#include "../include/akinator.h"

typedef tree_error_t error_t;

static long
get_file_size(FILE *const file)
{
   assert(file);
   
   long position = ftell(file);
   long size     = 0;
   
   fseek(file, 0L, SEEK_END);
   size = ftell(file);
   fseek(file, position, SEEK_SET);
   
   return size;
}

/// rename
static char*
find_string(char *const start)
{
   assert(start);
   
   for (char *ptr = start;; ptr++)
   {
      if (*ptr == '\n')
      {
         *ptr = '\0';
         return ptr + 1;
      }
      
      if (*ptr == '\0' || *ptr == EOF)
         return nullptr;
   }
}

error_t
database_to_tree(const char *const database_path, Tree *const p_tree)
{
   assert(database_path);
   assert(p_tree);
   
   FILE   *database      = nullptr;
   size_t  database_size = 0;
   size_t  n_nodes       = 0;
   char   *data          = nullptr;
   char   *string        = nullptr;
   
   database = fopen(database_path, "r");
   assert(database);
   
   database_size = get_file_size(database);
   data          = (char *)calloc(database_size, sizeof(char));
   assert(data);
   
   fread(data, sizeof(char), database_size, database);
   assert(fclose(database) == 0);
   
   sscanf(data, "%zu", &n_nodes);
   
   TreeNodeData  latestNode_data = {};
   TreeNode     *p_currentDaddy  = nullptr;
   TreeNode     *p_latestNode    = nullptr;
   
   string                 = find_string(data);
   latestNode_data.string = string + 1;
   p_currentDaddy         = tree_add_node(p_tree, &latestNode_data);
   
   for (size_t i = 1; i < n_nodes; i++)
   {
      string                 = find_string(string);
      latestNode_data.string = string + (*string == '?');
      p_latestNode           = tree_add_node(p_tree, &latestNode_data);
      
      if (p_currentDaddy->yeapSon == nullptr)
         tree_relate_nodes(p_currentDaddy, &p_currentDaddy->yeapSon, p_latestNode);
      else
         tree_relate_nodes(p_currentDaddy, &p_currentDaddy->nopeSon, p_latestNode);

      if (*string == '?')
         p_currentDaddy = p_latestNode;
      else if (i < n_nodes - 1) // not the last node
         while (p_currentDaddy->nopeSon != nullptr)
            p_currentDaddy = p_currentDaddy->daddy;
   }
   
   
   return TREE_NO_ERROR;
}


//11
//?Животное
//   ?Пресмыкающееся
//      ?Змея
//         Алябьева1
//         Алябьева2
//      ?Птица
//          Гнида1
//          Гнида2
//   ?Игра
//      Ведьмак 3
//      Скайрим
