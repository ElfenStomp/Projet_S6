#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "token_list.h"
#include "parser.h"

void token_comment(FILE *f, char ch, struct token* tok){
  int i = 0;
  tok->type = COMMENT;
  while((ch != '\n') && (i < TOKEN_SIZE-1) && (!feof(f))){
    tok->str[i] = ch;
    ch = getc(f);
    i++;
  }
  tok->str[i] = '\0';
}

void token_number(FILE *f, char ch, struct token* tok){
  int i = 0;
  tok->type = NUMBER;
  while (((ch =='-') || ((ch >= '0') && (ch <= '9'))) && (i < TOKEN_SIZE -1) && (!(feof(f)))){
    tok->str[i] = ch;
    ch = getc(f);
    i++;
  }
  tok->str[i] = '\0';
  fseek(f, -1, SEEK_CUR); //cursor goes back by 1 in the file
}

void token_space(FILE *f, char ch, struct token* tok){
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

void token_star(struct token* tok){
  tok->type = STAR;
  tok->str[0] = '*';
  tok->str[1] = '\0';
}

void token_return(struct token* tok){
  tok->type = RETURN;
  tok->str[0] = '\0';
}


void token_arrow(struct token* tok){
  tok->type = ARROW;
  tok->str[0] = '>';
  tok->str[1] = '\0';
}

void token_dollar(struct token* tok){
  tok->type = DOLLAR;
  tok->str[0] = '$';
  tok->str[1] = '\0';
}

void token_percent(struct token* tok){
  tok->type = PERCENT;
  tok->str[0] = '%';
  tok->str[1] = '\0';
}

void token_end(struct token* tok){
  tok->type = END_OF_FILE;
  tok->str[0] = '\0';
}

void token_card(FILE *f, char ch, struct token* tok){
  int i = 0;
  tok->type = CARD;
  while ((ch == '_') || ((toupper(ch) >= 'A') && (toupper(ch) <= 'Z') && (i < TOKEN_SIZE-1 ) && (!(feof(f))))){
    tok->str[i] = toupper(ch);
    ch = getc(f);
    i++;
  }
  tok->str[i] = '\0';
  fseek(f, -1, SEEK_CUR);
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

