#include <cctype>
#include "../include/database.h"
#include "../include/tree.h"

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

static inline char*
find_string(char *start)
{
   return start + strlen(start) + 1;
}

static inline char*
allocate_string(const char *const string)
{
   char *const allocString = (char *)calloc(strlen(string) + 1, sizeof(char));
   strcpy(allocString, string + (*string == '?'));
   
   return allocString;
}

static void
chop_strings(char *const start)
{
   assert(start);
   
   for (char *ptr = start; *ptr != '\0'; ptr++)
      if (*ptr == '\n')
         *ptr = '\0';
}

void
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
   TreeNode     *p_latestNode    = nullptr;
   TreeNode     *p_currentDaddy  = nullptr;
   
   chop_strings(data);
   
   string                 = find_string(data);
   latestNode_data.string = allocate_string(string);
   p_currentDaddy         = tree_add_node(p_tree, &latestNode_data);
   
   for (size_t i = 1; i < n_nodes; i++)
   {
      string                 = find_string(string);
      latestNode_data.string = allocate_string(string);
      p_latestNode           = tree_add_node(p_tree, &latestNode_data);
      
      if (p_currentDaddy->yeapSon == nullptr)
         tree_relate(p_currentDaddy, p_latestNode, &p_currentDaddy->yeapSon);
      else
         tree_relate(p_currentDaddy, p_latestNode, &p_currentDaddy->nopeSon);
      
      if (*string == '?')
         p_currentDaddy = p_latestNode;
      else if (i < n_nodes - 1) // not the last node
         while (p_currentDaddy->nopeSon != nullptr)
            p_currentDaddy = p_currentDaddy->daddy;
   }
   
   free(data);
}

void
tree_to_database(const char *const database_path, Tree *const p_tree)
{
   assert(database_path);
   assert(p_tree);
   
   FILE     *database = fopen(database_path, "w");
   TreeNode *p_node   = p_tree->root;
   
   assert(database);
   
   fprintf(database, "%zu\n", p_tree->size);
   
   for (size_t i = 0; i < p_tree->size; i++)
   {
      if (p_node->yeapSon == nullptr)
      {
         fprintf(database, "%s\n", p_node->data.string);
         
         if (p_node->daddy->yeapSon == p_node)
            p_node = p_node->daddy->nopeSon;
         else if (i < p_tree->size - 1)
         {
            while (p_node->daddy->nopeSon == p_node)
               p_node = p_node->daddy;
            
            p_node = p_node->daddy->nopeSon;
         }
      }
      else
      {
         fprintf(database, "?%s\n", p_node->data.string);
         p_node = p_node->yeapSon;
      }
   }
   assert(fclose(database) == 0);
}