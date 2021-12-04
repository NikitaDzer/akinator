#include <iconv.h>
#include <windows.h>
#include "../include/akinator.h"
#include "../include/database.h"
#include "../include/dump.h"

typedef tree_error_t error_t;

enum AkinatorMode
{
   SHIT    = 0,
   QUIT    = 1,
   GUESS   = 2,
   DEFINE  = 3,
   COMPARE = 4,
};

static void
get_input(char *const buffer)
{
   assert(buffer);
   
   static char  input[AKINATOR_BUFFER_SIZE] = "";
   char        *p_input                     = input;
   char        *p_buffer                    = buffer;
   
   size_t input_len  = 0;
   size_t buffer_len = AKINATOR_BUFFER_SIZE;
   
   fgets(input, AKINATOR_BUFFER_SIZE, stdin);
   
   if (ferror(stdin))
      return;
   
   input_len = strlen(input);
   input[--input_len] = '\0';
   memset(buffer, 0, AKINATOR_BUFFER_SIZE);
   
   iconv_t descriptor = iconv_open("UTF-8", "CP1251");
   iconv(descriptor, &p_input, &input_len, &p_buffer, &buffer_len);
   iconv_close(descriptor);
   
   input[0] = '\0';
}

static AkinatorMode
get_akinator_mode()
{
   char buffer[AKINATOR_BUFFER_SIZE] = "";
   
   printf("Выбери один из режимов <угадай>, <определи>, <сравни>. Напиши <выйти>, чтобы попрощаться со мной.\n");
   
   do
   {
      get_input(buffer);
   
      if (strncmp(buffer, "угадай", sizeof("угадай")) == 0)
         return AkinatorMode::GUESS;
      if (strncmp(buffer, "определи", sizeof("определи")) == 0)
         return AkinatorMode::DEFINE;
      if (strncmp(buffer, "сравни", sizeof("сравни")) == 0)
         return AkinatorMode::COMPARE;
      if (strncmp(buffer, "выйти", sizeof("выйти")) == 0)
         return AkinatorMode::QUIT;
      
      printf("Не понял тебя. Давай по новой.\n");
   }  while (true);
}

static inline bool
need_exit(const char *const buffer)
{
   return strncmp(buffer, "выйти", sizeof("выйти")) == 0;
}

static bool
need_continue()
{
   char buffer[AKINATOR_BUFFER_SIZE] = "";
   
   while (true)
   {
      printf("Желаешь продолжить?\n");
      get_input(buffer);
      
      if (need_exit(buffer))
         return false;
      
      if (strncmp(buffer, "да", sizeof("да")) == 0)
         return true;
      if (strncmp(buffer, "нет", sizeof("нет")) == 0)
         return false;
      
      printf("Блин, не понял.\n");
   }
}

static void
add_variant(Tree *const p_tree, TreeNode *const p_daddy, TreeNode *const p_transferNode)
{
   TreeNode **p_daddy_son = nullptr;
   TreeNode  *p_son       = nullptr;
   
   const TreeNodeData whoNode_data      = {
           .string = (char *)calloc(AKINATOR_BUFFER_SIZE * 2, sizeof(char))
   };
   const TreeNodeData questionNode_data = {
           .string = whoNode_data.string + AKINATOR_BUFFER_SIZE
   };
   
   printf("Ладно, и кто же это?\n");
   get_input(whoNode_data.string);
   
   if (need_exit(whoNode_data.string))
      return;
   
   printf("И чем же %s отличается от %s?\n", whoNode_data.string, p_transferNode->data.string);
   get_input(questionNode_data.string);
   
   if (need_exit(questionNode_data.string))
      return;
   
   if (p_daddy->yeapSon == p_transferNode)
      p_daddy_son = &p_daddy->yeapSon;
   else
      p_daddy_son = &p_daddy->nopeSon;
   
   tree_relate_nodes(p_daddy, p_daddy_son, tree_add_node(p_tree, &questionNode_data));
   tree_relate_nodes(*p_daddy_son, &(*p_daddy_son)->yeapSon, tree_add_node(p_tree, &whoNode_data));
   tree_relate_nodes(*p_daddy_son, &(*p_daddy_son)->nopeSon, p_transferNode);
}

static const TreeNode*
find_path(const TreeNode *const p_startNode, const char *const buffer)
{
   assert(p_startNode);
   assert(buffer);
   
   const TreeNode *p_node = p_startNode;
   
   while (true)
   {
      if (p_node->yeapSon == nullptr)
      {
         if (strcmp(buffer, p_node->data.string) == 0)
            return p_node;
         
         if (p_node->daddy->yeapSon == p_node)
            p_node = p_node->daddy->nopeSon;
         else
         {
            while (p_node->daddy->nopeSon == p_node)
            {
               if (p_node->daddy == p_startNode)
                  return nullptr;
               
               p_node = p_node->daddy;
            }
            
            p_node = p_node->daddy->nopeSon;
         }
      }
      else
         p_node = p_node->yeapSon;
   }
}

static void
free_data(Tree *const p_tree)
{
   assert(p_tree);
   
   const TreeNode *p_node = p_tree->root;
   
   while (true)
   {
      free(p_node->data.string);
      
      if (p_node->yeapSon == nullptr)
      {
         if (p_node->daddy->yeapSon == p_node)
            p_node = p_node->daddy->nopeSon;
         else
         {
            while (p_node->daddy->nopeSon == p_node)
            {
               if (p_node->daddy == p_tree->root)
                  return;
               
               p_node = p_node->daddy;
            }
            
            p_node = p_node->daddy->nopeSon;
         }
      }
      else
         p_node = p_node->yeapSon;
   }
}

static void
akinator_greetings()
{
   printf("Привет, Анонимус! Вновь хочешь убедиться, что я - всемогущий?\n");
}

static void
akinator_quit()
{
   printf("Было очень круто. Удачи, Бро!\n");
}

static void
akinator_guess(Tree *const p_tree)
{
   assert(p_tree);
   
   char buffer[AKINATOR_BUFFER_SIZE] = "";
   TreeNode *p_node = nullptr;
   
   while (true)
   {
      p_node = &p_tree->root[0];
      
      do
      {
         printf("%s?\n", p_node->data.string);
         get_input(buffer);
   
         if (need_exit(buffer))
            return;
         
         if (strncmp(buffer, "да", sizeof("да")) == 0)
            p_node = p_node->yeapSon;
         else if (strncmp(buffer, "нет", sizeof("нет")) == 0)
            p_node = p_node->nopeSon;
         else
            printf("Неизвестный ввод. Всё ок?\n");
      } while (p_node->yeapSon != nullptr);
   
      while (true)
      {
         printf("Ты загадал: %s. Верно?\n", p_node->data.string);
         get_input(buffer);
         
         if (need_exit(buffer))
            return;
         
         if (strncmp(buffer, "да", sizeof("да")) == 0)
         {
            printf("СЮДА!\n");
   
            if (!need_continue())
               return;
            
            break;
         }
         
         if (strncmp(buffer, "нет", sizeof("нет")) == 0)
         {
            add_variant(p_tree, p_node->daddy, p_node);
            break;
         }
         
         printf("Чел, ты можешь ответить только <да> или <нет>. Попробуй ещё раз, ты сможешь!\n");
      }
   }
}

static void
akinator_define(Tree *const p_tree)
{
   assert(p_tree);
   
   char buffer[AKINATOR_BUFFER_SIZE] = "";
   const TreeNode *p_node = nullptr;
   
   while (true)
   {
      while (true)
      {
         printf("О ком хочешь узнать?\n");
         get_input(buffer);
      
         if (need_exit(buffer))
            return;
         
         p_node = find_path(p_tree->root, buffer);
         
         if (p_node != nullptr)
            break;
         
         printf("Не обессуй, но я не смог найти твоего клиента. Повтори ввод.\n");
      }
   
      printf("%s", buffer);
   
      if (p_node->daddy->nopeSon == p_node)
         printf(" не");
      printf(" %s", p_node->daddy->data.string);
      
      p_node = p_node->daddy;
      
      while (p_node->daddy->daddy != nullptr)
      {
         printf(",");
         
         if (p_node->daddy->nopeSon == p_node)
            printf(" не");
         printf(" %s", p_node->daddy->data.string);
         
         p_node = p_node->daddy;
      }
      
      printf(" и");
   
      if (p_node->daddy->nopeSon == p_node)
         printf(" не");
      printf(" %s.\n", p_node->daddy->data.string);
   
      if (!need_continue())
         return;
   }
}

static void
akinator_compare(Tree *const p_tree)
{
   assert(p_tree);
   
   char who_1[AKINATOR_BUFFER_SIZE] = "";
   char who_2[AKINATOR_BUFFER_SIZE] = "";
   const TreeNode *p_node_1   = nullptr;
   const TreeNode *p_node_2   = nullptr;
   const TreeNode *p_sameNode = nullptr;
   const TreeNode *p_tempNode = nullptr;
   
   while (true)
   {
      while (true)
      {
         printf("Кого хочешь сравнить?\n");
         get_input(who_1);
         
         if (need_exit(who_1))
            return;
         
         p_node_1 = find_path(p_tree->root, who_1);
         
         if (p_node_1 != nullptr)
            break;
         
         printf("Такого в базе нет.\n");
      }
   
      while (true)
      {
         printf("С кем хочешь сравнить %s?\n", who_1);
         get_input(who_2);
         
         if (need_exit(who_2))
            return;
   
         p_node_2 = find_path(p_tree->root, who_2);
         
         if (p_node_2 != nullptr)
            break;
         
         printf("Нет такого в базе.\n");
      }
      
      if (p_node_1 != p_node_2)
      {
         p_sameNode = p_node_1->daddy;
         
         while (!find_path(p_sameNode, who_2))
            p_sameNode = p_sameNode->daddy;
      }
      else
         p_sameNode = p_node_1;
   
      if (p_sameNode->daddy)
      {
         p_tempNode = p_sameNode;
         printf("%s и %s", who_1, who_2);
         
         if (p_tempNode->daddy->nopeSon == p_tempNode)
            printf(" не");
         printf(" %s", p_tempNode->daddy->data.string);
   
         p_tempNode = p_tempNode->daddy;
         
         while (p_tempNode->daddy != nullptr)
         {
            printf(",");
      
            if (p_tempNode->daddy->nopeSon == p_tempNode)
               printf(" не");
            printf(" %s", p_tempNode->daddy->data.string);
      
            p_tempNode = p_tempNode->daddy;
         }
         
         if (p_node_1 != p_node_2)
            printf(", но ");
      }
      
      if (p_node_1 != p_node_2)
      {
         printf("%s", who_1);
         
         if (p_node_1->daddy->nopeSon == p_node_1)
            printf(" не");
         printf(" %s", p_node_1->daddy->data.string);
         
         p_node_1 = p_node_1->daddy;
         
         while (p_node_1 != p_sameNode && p_node_1->daddy != nullptr)
         {
            printf(",");
            
            if (p_node_1->daddy->nopeSon == p_node_1)
               printf(" не");
            printf(" %s", p_node_1->daddy->data.string);
            
            p_node_1 = p_node_1->daddy;
         }
   
         printf(", %s в свою очередь", who_2);
   
         if (p_node_2->daddy->nopeSon == p_node_2)
            printf(" не");
         printf(" %s", p_node_2->daddy->data.string);
         
         p_node_2 = p_node_2->daddy;
         
         while (p_node_2 != p_sameNode && p_node_2->daddy != nullptr)
         {
            printf(",");
            
            if (p_node_2->daddy->nopeSon == p_node_2)
               printf(" не");
            printf(" %s", p_node_2->daddy->data.string);
            
            p_node_2 = p_node_2->daddy;
         }
      }
      
      printf(".\n");
      
      if (!need_continue())
         return;
   }
}

error_t
akinator(Tree *const p_tree)
{
   assert(p_tree);
   
   SetConsoleCP(1251);
   SetConsoleOutputCP(65001);
   
   char buffer[AKINATOR_BUFFER_SIZE] = "";
   AkinatorMode akinator_mode = AkinatorMode::SHIT;
   
   database_to_tree(DATABASE_DEFAULT_PATH, p_tree);
   akinator_greetings();
   
   do
   {
      akinator_mode = get_akinator_mode();
      
      switch (akinator_mode)
      {
         case AkinatorMode::GUESS:
         {
            akinator_guess(p_tree);
            break;
         }
         
         case AkinatorMode::DEFINE:
         {
            akinator_define(p_tree);
            break;
         }
         
         case AkinatorMode::COMPARE:
         {
            akinator_compare(p_tree);
            break;
         }
         
         default: break;
      }

   } while (akinator_mode != AkinatorMode::QUIT);
   
   akinator_quit();
   tree_to_database(DATABASE_DEFAULT_PATH, p_tree);
   tree_dump(p_tree);
   
   free_data(p_tree);
   
   return TREE_NO_ERROR;
}
