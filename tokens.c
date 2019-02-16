#include "tokens.h"


//returns whether the given character is an operator
int notOp(char op)
{
   if(op == '<' || op == '>' || op == '|'
   || op == '&' || op == ';')
   {
      return 0;
   }

   return 1;
}

//takes a string, tokenizes it, and stores each token in the given vector
void tokenize(char* line, vec * tokens)
{
   int size = strlen(line);

   //count is the amount of characters in the token
   int count = 0;
   //int isQuote = 0;
   //iterate through string
   for(int i = 0; i <= size; i++)
   {
      // if (line[i] == '(' || line[i] == ')')
      // {
      //    char paren[2] = {line[i], '\0'};
      //    push_vec(tokens,paren);
      //    if (count == 0)
      //    {
      //       continue;
      //    }
      // }
      // if (line[i] == '"' && !isQuote)
      // {
      //    isQuote = 1;
      // }

      // else if(line[i] == '"' && isQuote)
      // {
      //    isQuote = 0;
      // }

      if (line[i] != 9 && line[i] != ' ' && notOp(line[i]) && line[i] != '\n' /*|| isQuote*/)
      {  
         //char is part of a token, so update count
         count++;
      }

      //put token into vector
      else if ( count != 0 /*|| (!isQuote && line[i] == '"')*/) 
      {
         char *token = calloc(count + 1, sizeof(char));
         char *ptr = &line[i - count];
         strncpy(token, ptr, count);
         push_vec(tokens, token);
         free(token);
         //push to vec
         count = 0;  
      }

      //if an operator, put in vector
      if (!notOp(line[i]) /*&& !isQuote*/)
      {
         if ((line[i] != '|' && line[i] != '&') || line[i+1] != line[i])
         {
            char token[2];
            token[0] = line[i];
            token[1] = '\0';
            push_vec(tokens, token);
         }

         else if (line[i+1] == line[i])
         {
            char token[3];
            char *ptr = &line[i];
            strncpy(token, ptr, 2);
            token[2] = '\0';
            push_vec(tokens, token);
            i++;
         }
      }
      
   }
}



