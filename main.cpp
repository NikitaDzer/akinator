#include <cstdio>
#include "include/tree.h"
#include "include/dump.h"
#include "include/akinator.h"
#include "include/config.h"

int main()
{
   Tree tree = {0};

   tree_construct(&tree, 10);

   akinator(&tree);
   
   tree_destruct(&tree);
   
   return 0;
}
