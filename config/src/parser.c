#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "token_list.h"
#include "parser.h"

/*
  Lexer
  POST: tok contains the next token from stream f. The token
  value (str) is truncated at TOKEN_SIZE-1 bytes.
*/

void lexer(FILE *f, struct token* tok){
  char ch = getc(f);
  if (ch == '#')
    token_comment(f, ch, tok);
  else if (ch == '-' || ((ch >= '0') && (ch <= '9')))
    token_number(f, ch, tok);
  else if (ch == ' ') 
    token_space(f, ch, tok);
  else if (ch == '*')
    token_star(tok);
  else if (ch == '\n')
    token_return(tok);
  else if (ch == '>')
    token_arrow(tok);
  else if (ch == '$')
    token_dollar(tok);
  else if (ch == '%')
    token_percent(tok);
  else if (feof(f))
    token_end(tok);
  else if ((toupper(ch) >= 'A') && (toupper(ch) <= 'Z'))
    token_card(f, ch, tok);
}

/*
  Returns a token list containing all tokens of a given file 
 */

struct token_list* build_token_list(FILE *f){
  struct token_list* l = empty_list();
  struct token tok;
  while (!feof(f)){
    tok.type = NONE;
    lexer(f,&tok);
    if (tok.type != NONE)
      add_token(l,tok);
  }
  return l;
}

/*
  returns FILE_OK if file is a correct sapotache configuration file,
  FILE_CORRUPT otherwise
 */

int check_if_valid(FILE* f){
  struct token_list* m = build_token_list(f);
  struct token_list* l = remove_token_by_type(m, COMMENT); //removes comments from list
  free_list(m);
  
  if (is_empty(l)){
    free_list(l);
    {free_list(l);return FILE_CORRUPT;}
  }

  struct lelement* e = l->head;

  int col = 0;
  int height = 0;
  int width = 0;
  int lines = 0;
  int holes = 0;
  int start = 0;
  int stars = 0;
  int finish = 0;
  
  //File must begin with NUMBER SPACE NUMBER RETURN
  if (e->t.type != NUMBER)
    {free_list(l);return FILE_CORRUPT;}
  width = atoi(e->t.str);
  e = e->next;
  if (e->t.type != SPACE)
    {free_list(l);return FILE_CORRUPT;}
  e = e->next;
  if (e->t.type != NUMBER)
    {free_list(l);return FILE_CORRUPT;}
  height = atoi(e->t.str);
  e = e->next;
  if (e->t.type != RETURN)
    {free_list(l);return FILE_CORRUPT;}

  if (width <= 0 || height <= 0)
    {free_list(l);return FILE_CORRUPT;}
  
  //moving to board specification
  while (!(e->t.type == STAR || e->t.type == ARROW || e->t.type == DOLLAR || e->t.type == PERCENT))
    e = e->next;

  while ((lines < height) && (e->t.type == STAR || e->t.type == ARROW || e->t.type == DOLLAR || e->t.type == PERCENT || e->t.type == RETURN)){

    if (!(e->t.type == STAR || e->t.type == ARROW || e->t.type == DOLLAR || e->t.type == PERCENT || e->t.type == RETURN))
      {free_list(l);return FILE_CORRUPT;}
    
    if (e->t.type == STAR){
      stars++;
      col++;
      e = e->next;
      continue;
    }
    if (e->t.type == ARROW){
      start++;
      col++;
      e = e->next;
      continue;
    }
    if (e->t.type == DOLLAR){
      finish++;
      col++;
      e = e->next;
      continue;
    }
    if (e->t.type == PERCENT){
      holes++;
      col++;
      e = e->next;
      continue;
    }
    if (e->t.type == RETURN){
      lines++;
      if (col != width)
	{free_list(l);return FILE_CORRUPT;}
      col = 0;
      e = e->next;
    }
  }

  //At least one finishing point
  if (finish < 1)
    {free_list(l);return FILE_CORRUPT;}
  //Exactly one starting point
  if (start != 1)
    {free_list(l);return FILE_CORRUPT;}
  
  if (lines != height)
    {free_list(l);return FILE_CORRUPT;}

    //Next token should be a card
  while (e->t.type != CARD){
    if (!(e->t.type == RETURN || e->t.type == SPACE))
      {free_list(l);return FILE_CORRUPT;}
    e = e->next;
  }
  //All following sequences of tokens must be CARD SPACE NUMBER RETURN
  while (e->t.type != END_OF_FILE){
    if (e->t.type != CARD){
      {free_list(l);return FILE_CORRUPT;}
    }
    if (!(check_card(e->t.str)))
      {free_list(l);return FILE_CORRUPT;}
    e = e->next;
    
    if (e->t.type != SPACE)
      {free_list(l);return FILE_CORRUPT;}
    e = e->next;

    if (e->t.type != NUMBER)
      {free_list(l);return FILE_CORRUPT;}
    if (atoi(e->t.str) <= 0)
      {free_list(l);return FILE_CORRUPT;}
    e = e->next;

    if (e->t.type != RETURN)
      {free_list(l);return FILE_CORRUPT;}

    while (e->t.type == RETURN) //possibly more than one RETURN
      e = e->next;
  }
  free_list(l);  
  return FILE_OK;
}

/*
Create the file (l must be checked before)
*/
void parse(struct token_list* l){
   FILE * f = fopen("test", "w");
   if (f == NULL) {
     printf("*** cannot open file \n");
     return;
   }
   struct lelement* e = l->head;
   e = write_configuration(f, e);
   e = write_card_types(f, e);
   e = write_nb_cards(f, e);
   e = write_objectives(f, e);
   e = write_holes(f, e);
   e = write_allow_boulder(f, e);
   e = write_allow_breaks(f, e);
   e = write_repair_break(f, e);
     
   free_list(l);
   fclose(f);
} 
