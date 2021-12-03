#include <cstdio>
#include "include/tree.h"
#include "include/dump.h"
#include "include/akinator.h"
#include "include/config.h"

int main()
{
   Tree tree = {0};

   tree_construct(&tree, 10);
   database_to_tree(DATABASE_DEFAULT_PATH, &tree);

   akinator(&tree);
   
   tree_to_database(DATABASE_DEFAULT_PATH, &tree);
   
   tree_dump(&tree);
   tree_destruct(&tree);
   
   return 0;
}
