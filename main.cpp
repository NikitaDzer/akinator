#include <cstdio>
#include "include/tree.h"
#include "include/dump.h"
#include "include/akinator.h"
#include "include/config.h"

int main()
{
   Tree *const p_tree = tree_construct();
   akinator(p_tree);
   tree_destruct(p_tree);
   
   return 0;
}
