//
// Created by User on 25.11.2021.
//

#include "../include/dump.h"
#include "../include/tree.h"

static FILE   *dotfile = nullptr;
static FILE   *stream  = nullptr;
static size_t  n_dumps = 0;

static void
dump_open()
{
   stream = fopen(TREE_DUMP_PATH, "w");
   assert(stream);
   
   fprintf(stream,
           "<!doctype html>                                                             \n"
           "<html>                                                                      \n"
           "\t<head lang='en'>                                                          \n"
           "\t\t<meta charset='utf-8'>                                                  \n"
           "\t\t<meta name='viewport' content='width=device-width'>                     \n"
           "\t\t<title>Tree dump</title>                                                \n"
           "\t\t<style>                                                                 \n"
           "\t\tbody {                                                                  \n"
           "\t\t\tbackground: #222222;                                                  \n"
           "\t\t\tcolor:      #DDDDDD;                                                  \n"
           "\t\t\tfont-family: Arial, sans-serif;                                       \n"
           "\t\t}                                                                       \n"
           "\t\t.tree {                                                                 \n"
           "\t\t\twidth: max-content;                                                   \n"
           "\t\t\tborder: 3px solid #AAAAAA;                                            \n"
           "\t\t\tfont-size: 20px;                                                      \n"
           "\t\t\tpadding: 10px 20px;                                                   \n"
           "\t\t}                                                                       \n"
           "\t\t.tree_img {                                                             \n"
           "\t\t\toverflow-x: auto;                                                     \n"
           "\t\t}                                                                       \n"
           "\t\t</style>                                                                \n"
           "\t</head>                                                                   \n"
           "\t<body>                                                                    \n");
}

static void
dump_close()
{
   fprintf(stream,
           "\t</body>\n"
           "</html>\n");
   
   assert(fclose(stream) == 0);
}

static void
dump_controller()
{
   if (stream == nullptr)
   {
      dump_open();
      atexit(dump_close);
   }
}

static void
tree_to_digraph(const Tree *const p_tree)
{
   assert(p_tree);
   
   dotfile = fopen(TREE_DOTFILE_PATH, "w");
   assert(dotfile);
   
   fprintf(dotfile,
           "digraph G                                                                                      \n"
           "{                                                                                              \n"
           "\tbgcolor  = \"#222222\"                                                                       \n"
           "\tcompound = true                                                                              \n"
           "\tnewrank  = true                                                                              \n"
           "\tgraph[dpi = 250, splines = polyline, nodesep = 0.3, ranksep = 1, rankdir = TB]               \n"
           "\tnode[shape = record, style = \"filled\", penwidth = 2, color = \"#333333\", margin = 0.04,   \n"
           "\tfontcolor = \"white\", fontsize = 8, fillcolor = \"#666666\", fontname = \"Consolas\"]       \n"
           "\tedge[arrowsize = 0.7, style = solid, penwidth = 1.1, color = \"#CCCCCC\"]                    \n");
   
   const TreeNode *p_node = p_tree->root;
   
   fprintf(dotfile,
           "\t\tnode_%p[label = \"%s\"]\n",
           p_node, p_node->data.string);
   p_node = p_node->yeapSon;
   
   for (size_t i = 1; i < p_tree->size; i++)
   {
      fprintf(dotfile,
              "\t\tnode_%p[label = \"%s\"]\n",
              p_node, p_node->data.string);
      
      if (p_node->daddy->yeapSon == p_node)
         fprintf(dotfile,
                 "\t\tnode_%p -> node_%p[label = \"yeap\" fontcolor = \"#BBBBBB\" fontsize = \"8\"]\n",
                 p_node->daddy, p_node);
      else
         fprintf(dotfile,
                 "\t\tnode_%p -> node_%p[label = \"nope\" fontcolor = \"#BBBBBB\" fontsize = \"8\"]\n",
                 p_node->daddy, p_node);
      
      if (i != p_tree->size - 1)
      {
         if (p_node->yeapSon == nullptr)
         {
            if (p_node->daddy->yeapSon == p_node)
               p_node = p_node->daddy->nopeSon;
            else
            {
               while (p_node->daddy->nopeSon == p_node)
                  p_node = p_node->daddy;
         
               p_node = p_node->daddy->nopeSon;
            }
         }
         else
            p_node = p_node->yeapSon;
      }
   }
   
   fprintf(dotfile, "}\n");
   assert(fclose(dotfile) == 0);
}

void
tree_dump(const Tree *const p_tree)
{
   assert(p_tree);
   
   char tree_png_path[64] = "tree_";
   char system_cmd[128]   = "dot ";
   
   tree_to_digraph(p_tree);
   
   sprintf(tree_png_path, "%zu.png", n_dumps);
   strcat(system_cmd, TREE_DOTFILE_PATH);
   strcat(system_cmd, " -Tpng -o ");
   strcat(system_cmd, tree_png_path);
   
   system(system_cmd);
   
   dump_controller();
   
   fprintf(stream,
           "\t\t<hr width = '100%'>                                     \n"
           "\t\t<div class = 'tree'>                                    \n"
           "\t\t<pre>Tree  address:  %p</pre>                           \n"
           "\t\t<pre>Root  address:  %p</pre>                           \n"
           "\t\t<pre>Free  address:  %p</pre>                           \n"
           "\t\t<pre>Final address:  %p</pre>                           \n"
           "\t\t<pre>Size:           %zu</pre>                          \n"
           "\t\t<pre>Capacity:       %zu</pre>                          \n"
           "\t\t</div>                                                  \n"
           "\t\t<div class = 'tree_img'>                                \n"
           "\t\t<img src = \"%s\" width = 100%>                         \n"
           "\t\t</div>                                                  \n",
           p_tree,
           p_tree->root,
           p_tree->free,
           p_tree->final,
           p_tree->size,
           p_tree->capacity,
           tree_png_path);
   
   n_dumps += 1;
}