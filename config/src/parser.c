#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "token_list.h"
#include "parser.h"

/*
  Lexer
  POST: tok contains the next token from stream f. The token
  value (str) is truncated at TOKEN_SIZE-1 bytes.
*/

void lexer(FILE *f, struct token* tok){
  int i = 0;
  char ch = getc(f);
  if (ch == '#'){
    tok->type = COMMENT;
    while((ch != '\n') && (i < TOKEN_SIZE-1) && (!feof(f))){
      tok->str[i] = ch;
      ch = getc(f);
      i++;
    }
    tok->str[i] = '\0';
  }
  else if (ch == '-' || ((ch >= '0') && (ch <= '9'))){
    tok->type = NUMBER;
    while (((ch =='-') || ((ch >= '0') && (ch <= '9'))) && (i < TOKEN_SIZE -1) && (!(feof(f)))){
      tok->str[i] = ch;
      ch = getc(f);
      i++;
    }
    tok->str[i] = '\0';
    fseek(f, -1, SEEK_CUR); //cursor goes back by 1 in the file
  }
  else if (ch == ' '){ 
    while (ch == ' ')    //note that multiple spaces count as one
      ch = getc(f);
    if (ch == '\n' || feof(f)){ //empty lines are ignored
      return;
    }
    fseek(f, -1, SEEK_CUR);
    tok->type = SPACE;
    tok->str[0] = ' ';
    tok->str[1] = '\0';
  }
  else if (ch == '*'){
    tok->type = STAR;
    tok->str[0] = '*';
    tok->str[1] = '\0';
  }
  else if (ch == '\n'){
    tok->type = RETURN;
    tok->str[0] = '\0';
  }
  else if (ch == '>'){
    tok->type = ARROW;
    tok->str[0] = '>';
    tok->str[1] = '\0';
  }
  else if (ch == '$'){
    tok->type = DOLLAR;
    tok->str[0] = '$';
    tok->str[1] = '\0';
  }
  else if (ch == '%'){
    tok->type = PERCENT;
    tok->str[0] = '%';
    tok->str[1] = '\0';
  }
  else if (feof(f)){
    tok->type = END_OF_FILE;
    tok->str[0] = '\0';
  }
  else if ((ch >= 'A') && (ch <= 'Z')){
    tok->type = CARD;
    while ((ch == '_') || ((ch >= 'A') && (ch <= 'Z') && (i < TOKEN_SIZE-1 ) && (!(feof(f))))){
      tok->str[i] = ch;
      ch = getc(f);
      i++;
    }
    tok->str[i] = '\0';
    fseek(f, -1, SEEK_CUR);
  }
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
  returns 1 if card is a valid sapotache card, 0 otherwise
 */

int check_card(char* card){
  //Tab that contains all valid card types
  char* cards[] = {"V_LINE","H_LINE","V_CROSS","H_CROSS","X_CROSS","L_TURN","R_TURN","D_END","BOULDER","B_AXE","B_LAMP","B_CART","R_AXE","R_LAMP","R_CART","R_ALL"};
  int len = 16;
  for (int i = 0; i < len; i++)
    if (!(strcmp(card,cards[i])))
      return 1;
  return 0;
}

/*
  returns FILE_OK if file is a correct sapotache configuration file,
  FILE_CORRUPT otherwise
 */

int check_if_valid(FILE* f){
  struct token_list* l = build_token_list(f);
  l = remove_token_by_type(l, COMMENT); //removes comments from list
  if (is_empty(l))
    return FILE_CORRUPT;
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
    return FILE_CORRUPT;
  width = atoi(e->t.str);
  e = e->next;
  if (e->t.type != SPACE)
    return FILE_CORRUPT;
  e = e->next;
  if (e->t.type != NUMBER)
    return FILE_CORRUPT;
  height = atoi(e->t.str);
  e = e->next;
  if (e->t.type != RETURN)
    return FILE_CORRUPT;

  if (width <= 0 || height <= 0)
    return FILE_CORRUPT;
  
  //moving to board specification
  while (!(e->t.type == STAR || e->t.type == ARROW || e->t.type == DOLLAR || e->t.type == PERCENT))
    e = e->next;

  while ((lines < height) && (e->t.type == STAR || e->t.type == ARROW || e->t.type == DOLLAR || e->t.type == PERCENT || e->t.type == RETURN)){

    if (!(e->t.type == STAR || e->t.type == ARROW || e->t.type == DOLLAR || e->t.type == PERCENT || e->t.type == RETURN))
      return FILE_CORRUPT;
    
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
	return FILE_CORRUPT;
      col = 0;
      e = e->next;
    }
  }

  //At least one finishing point
  if (finish < 1)
    return FILE_CORRUPT;
  //Exactly one starting point
  if (start != 1)
    return FILE_CORRUPT;
  
  if (lines != height)
    return FILE_CORRUPT;
  
  //Next token should be a card
  while (e->t.type != CARD){
    if (!(e->t.type == RETURN || e->t.type == SPACE))
      return FILE_CORRUPT;
    e = e->next;
  }

  //All following sequences of tokens must be CARD SPACE NUMBER RETURN
  while (e->t.type != END_OF_FILE){
    if (e->t.type != CARD){
      return FILE_CORRUPT;
    }
    if (!(check_card(e->t.str)))
      return FILE_CORRUPT;
    e = e->next;
    
    if (e->t.type != SPACE)
      return FILE_CORRUPT;
    e = e->next;

    if (e->t.type != NUMBER)
      return FILE_CORRUPT;
    if (atoi(e->t.str) <= 0)
      return FILE_CORRUPT;
    e = e->next;

    if (e->t.type != RETURN)
      return FILE_CORRUPT;

    while (e->t.type == RETURN) //possibly more than one RETURN
      e = e->next;
  }
  
  return FILE_OK;
  
}

/* void parse(FILE* f){ */
  
/* } */
