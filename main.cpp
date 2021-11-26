#include <cstdio>
#include <windows.h>
#include "include/tree.h"
#include "include/dump.h"
#include "include/akinator.h"

int main()
{
   Tree tree = {0};
      
   SetConsoleOutputCP(65001);
   
   tree_construct(&tree, 100);
   database_to_tree(DATABASE_DEFAULT_PATH, &tree);
   
   tree_dump(&tree);
   tree_destruct(&tree);
   
   return 0;
}
