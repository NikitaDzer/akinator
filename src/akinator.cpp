//
// Created by User on 25.11.2021.
//

#include <iconv.h>
#include <windows.h>
#include "../include/akinator.h"

typedef tree_error_t error_t;

enum UserResponse
{
   SHIT  = 0,
   QUIT  = 1,
   RESPONSE_YEAP  = 2,
   RESPONSE_NOPE  = 3,
   MODE_GUESS   = 4,
   MODE_DEFINE  = 5,
   MODE_COMPARE = 6,
};

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

static UserResponse
get_akinator_mode()
{
   char buffer[AKINATOR_BUFFER_SIZE] = "";
   
   printf("Выбери один из режимов <угадай>, <определи>, <сравни>. Напиши <выйти>, чтобы попрощаться со мной.\n");
   
   do
   {
      get_input(buffer);
   
      if (strncmp(buffer, "угадай", sizeof("угадай")) == 0)
         return UserResponse::MODE_GUESS;
      if (strncmp(buffer, "определи", sizeof("определи")) == 0)
         return UserResponse::MODE_DEFINE;
      if (strncmp(buffer, "сравни", sizeof("сравни")) == 0)
         return UserResponse::MODE_COMPARE;
      if (strncmp(buffer, "выйти", sizeof("выйти")) == 0)
         return UserResponse::QUIT;
      
      printf("Не понял тебя. Давай по новой.\n");
   }  while (true);
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
   
   printf("Ладно, кого ты загадал?\n");
   get_input(whoNode_data.string);
   
   printf("И чем же %s отличается от %s?\n", whoNode_data.string, p_transferNode->data.string);
   get_input(questionNode_data.string);
   
   if (p_daddy->yeapSon == p_transferNode)
      p_daddy_son = &p_daddy->yeapSon;
   else
      p_daddy_son = &p_daddy->nopeSon;
   
   tree_relate_nodes(p_daddy, p_daddy_son, tree_add_node(p_tree, &questionNode_data));
   tree_relate_nodes(*p_daddy_son, &(*p_daddy_son)->yeapSon, tree_add_node(p_tree, &whoNode_data));
   tree_relate_nodes(*p_daddy_son, &(*p_daddy_son)->nopeSon, p_transferNode);
}

static void
akinator_greetings()
{
   printf("Привет, Анонимус! Вновь хочешь убедиться, что я - всемогущий?\n");
}

static void
akinator_quit()
{
   printf("Круто повеселись. Удачи, Бро!\n");
}

static inline bool
need_exit(const char *const buffer)
{
   return strncmp(buffer, "выйти", sizeof("выйти")) == 0;
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
            printf("Неизвестный ввод. Ты дэцл?\n");
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
   
            while (true)
            {
               printf("Желаешь продолжить?\n");
               get_input(buffer);
   
               if (need_exit(buffer))
                  return;
               
               if (strncmp(buffer, "да", sizeof("да")) == 0)
                  break;
               if (strncmp(buffer, "нет", sizeof("нет")) == 0)
                  return;
               printf("Ты можешь нормально ответить? ");
            }
            
            break;
         }
         
         if (strncmp(buffer, "нет", sizeof("нет")) == 0)
         {
            add_variant(p_tree, p_node->daddy, p_node);
            break;
         }
         else
            printf("Чел, ты можешь ответить только <да> или <нет>. Попробуй ещё раз, ты сможешь!\n");
      }
   }
}

static void
akinator_define(Tree *const p_tree)
{
   assert(p_tree);
   
   char buffer[AKINATOR_BUFFER_SIZE] = "";
   bool is_path_found     = false;
   const TreeNode *p_node = nullptr;
   
   while (true)
   {
      while (true)
      {
         printf("О ком хочешь узнать?\n");
         get_input(buffer);
      
         if (need_exit(buffer))
            return;
         
         p_node = p_tree->root;
         
         for (size_t i = 0; i < p_tree->size; i++)
         {
            if (p_node->yeapSon == nullptr)
            {
               if (strcmp(buffer, p_node->data.string) == 0)
               {
                  is_path_found = true;
                  break;
               }
            
               if (i != p_tree->size - 1)
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
            }
            else
               p_node = p_node->yeapSon;
         }
   
         if (is_path_found)
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
      
      while (true)
      {
         printf("Желаешь продолжить?\n");
         get_input(buffer);
         
         if (need_exit(buffer))
            return;
         
         if (strncmp(buffer, "да", sizeof("да")) == 0)
            break;
         if (strncmp(buffer, "нет", sizeof("нет")) == 0)
            return;
         
         printf("Блин, не понял.\n");
      }
   }
}

//static void
//akinator_compare(Tree *const p_tree)
//{
//   assert(p_tree);
//
//   char who_1[AKINATOR_BUFFER_SIZE] = "";
//   char who_2[AKINATOR_BUFFER_SIZE] = "";
//   bool is_path_found     = false;
//   const TreeNode *p_node = nullptr;
//
//   while (true)
//   {
//      while (true)
//      {
//         printf("Кого с кем хочешь сравнить?\n");
//         get_input(buffer);
//
//         p_node = p_tree->root;
//
//         for (size_t i = 0; i < p_tree->size; i++)
//         {
//            if (p_node->yeapSon == nullptr)
//            {
//               if (strcmp(buffer, p_node->data.string) == 0)
//               {
//                  is_path_found = true;
//                  break;
//               }
//
//               if (i != p_tree->size - 1)
//               {
//                  if (p_node->daddy->yeapSon == p_node)
//                     p_node = p_node->daddy->nopeSon;
//                  else
//                  {
//                     while (p_node->daddy->nopeSon == p_node)
//                        p_node = p_node->daddy;
//
//                     p_node = p_node->daddy->nopeSon;
//                  }
//               }
//            }
//            else
//               p_node = p_node->yeapSon;
//         }
//
//         if (is_path_found)
//            break;
//
//         printf("Не обессуй, но я не смог найти твоего клиента. Повтори ввод.\n");
//      }
//
//      printf("%s", buffer);
//
//      if (p_node->daddy->nopeSon == p_node)
//         printf(" не");
//      printf(" %s", p_node->daddy->data.string);
//
//      p_node = p_node->daddy;
//
//      while (p_node->daddy->daddy != nullptr)
//      {
//         printf(",");
//
//         if (p_node->daddy->nopeSon == p_node)
//            printf(" не");
//         printf(" %s", p_node->daddy->data.string);
//
//         p_node = p_node->daddy;
//      }
//
//      printf(" и");
//
//      if (p_node->daddy->nopeSon == p_node)
//         printf(" не");
//      printf(" %s.\n", p_node->daddy->data.string);
//
//      while (true)
//      {
//         printf("Желаешь продолжить?\n");
//         get_input(buffer);
//
//         if (strncmp(buffer, "да", sizeof("да")) == 0)
//            break;
//         if (strncmp(buffer, "нет", sizeof("нет")) == 0)
//            return;
//
//         printf("Блин, не понял.\n");
//      }
//   }
//}

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
   TreeNode     *p_latestNode    = nullptr;
   TreeNode     *p_currentDaddy  = nullptr;
   
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
   
   string[strlen(string) - 1] = '\0';
   
   return TREE_NO_ERROR;
}

error_t
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
   
   return TREE_NO_ERROR;
}

error_t
akinator(Tree *const p_tree)
{
   assert(p_tree);
   
   SetConsoleCP(1251);
   SetConsoleOutputCP(65001);
   
   UserResponse akinator_mode = UserResponse::SHIT;
   char buffer[AKINATOR_BUFFER_SIZE] = "";
   
   akinator_greetings();
   
   do
   {
      akinator_mode = get_akinator_mode();
      
      switch (akinator_mode)
      {
         case UserResponse::MODE_GUESS:
         {
            akinator_guess(p_tree);
            break;
         }
         
         case UserResponse::MODE_DEFINE:
         {
            akinator_define(p_tree);
            break;
         }
         
//         case UserResponse::MODE_COMPARE:
//         {
//            akinator_compare(p_tree);
//            break;
//         }
         
         default: break;
      }

   } while (akinator_mode != UserResponse::QUIT);
   
   akinator_quit();
   
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
