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
    return FILE_CORRUPT;
  }

  struct lelement* e = l->head;
  
  int board = check_board(e);
  if(board == FILE_CORRUPT){
    free_list(l);
    return FILE_CORRUPT;
  }
  
  int deck = check_deck(e);
  free_list(l);  
  return deck;

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
