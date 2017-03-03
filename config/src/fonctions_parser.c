#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "token_list.h"
#include "parser.h"
//_________________________________________________________________________________________
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
//_________________________________________________________________________________________

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

//_________________________________________________________________________________________
struct lelement * write_configuration(FILE* f, struct lelement *e){
  fprintf(f, "Configuration : %sx%s\n", e->next->next->t.str, e->t.str);
  return e->next->next->next;
}

struct lelement * write_card_types(FILE* f, struct lelement *e){
  struct lelement *d = e;
  int k = 0;
  fprintf(f, "Card types    : ");
  while(e->t.type != END_OF_FILE){
    if(e->t.type == CARD)
      k++;
    e = e->next;
  }
  fprintf(f, "%d\n", k);
  return d;
}

struct lelement * write_nb_cards(FILE* f, struct lelement *e){
  struct lelement *d = e;
  int k = 0;
  fprintf(f, "Nb of cards   : ");
  while(e->t.type != END_OF_FILE){
    if(e->t.type == CARD){
      e = e->next->next;
      k = k + atoi(e->t.str);
      }
    e = e->next;
  }
  fprintf(f, "%d\n", k);
  return d;
}

struct lelement * write_objectives(FILE* f, struct lelement *e){
  e = e->next;
  int nb_obj = 0;
  int line = 0;
  int column = 0;
  int width = 0;
  struct lelement *d;

  fprintf(f, "Objectives    :");
  while (!(e->t.type == STAR || e->t.type == ARROW || e->t.type == DOLLAR || e->t.type == PERCENT))
    e = e->next;
  d = e;
  
  while (e->t.type == STAR || e->t.type == ARROW || e->t.type == DOLLAR || e->t.type == PERCENT || e->t.type == RETURN){
    if(e->t.type == DOLLAR){
      nb_obj++;
    }
    if (e->t.type == RETURN){
      line++;
      width++;
      column = 0;
    }
    else{
      column++;
    }
    e = e->next;
  }
  
  column = 0;
  line = 0;
  fprintf(f, " %d ", nb_obj);
  e = d;
  
  while (e->t.type == STAR || e->t.type == ARROW || e->t.type == DOLLAR || e->t.type == PERCENT || e->t.type == RETURN){
    if(e->t.type == DOLLAR){
      fprintf(f, "(%d, %d) ", column, width - line - 1);
    }
    if (e->t.type == RETURN){
      line++;
      column = 0;
    }
    else{
      column++;
    }
    e = e->next;
  }
  fprintf(f, "\n");
  return d;
}

struct lelement * write_holes(FILE* f, struct lelement *e){
  int nb_holes = 0;
  int line = 0;
  int column = 0;
  int width = 0;
  struct lelement *d = e;

  fprintf(f, "Holes         :");
  while (e->t.type == STAR || e->t.type == ARROW || e->t.type == DOLLAR || e->t.type == PERCENT || e->t.type == RETURN){
    if(e->t.type == PERCENT){
      nb_holes++;
    }
    if (e->t.type == RETURN){
      line++;
      width++;
      column = 0;
    }
    else{
      column++;
    }
    e = e->next;
  }
  
  column = 0;
  line = 0;
  fprintf(f, " %d ", nb_holes);
  e = d;
  
  while (e->t.type == STAR || e->t.type == ARROW || e->t.type == DOLLAR || e->t.type == PERCENT || e->t.type == RETURN){
    if(e->t.type == PERCENT){
      fprintf(f, "(%d, %d) ", column, width - line - 1);
    }
    if (e->t.type == RETURN){
      line++;
      column = 0;
    }
    else{
      column++;
    }
    e = e->next;
  }
  fprintf(f, "\n");
  return e;
}
  
struct lelement * write_allow_boulder(FILE* f, struct lelement *e){
  struct lelement *d = e;
  int k = 0;
  fprintf(f, "Allow boulder : ");
  while(e->t.type != END_OF_FILE){
    if(e->t.type == CARD){
      if(!strcmp(e->t.str, "BOULDER")){
	k = 1;
	fprintf(f, "yes\n");
	break;
      }
    }
    e = e->next;
  }
  if(!k)
    fprintf(f, "no\n");
  return d;
}

struct lelement * write_allow_breaks(FILE* f, struct lelement *e){
  struct lelement *d = e;
  int k = 0;
  fprintf(f, "Allow breaks  : ");
  while(e->t.type != END_OF_FILE){
    if(e->t.type == CARD){
      if((e->t.str[0] == 'B') && (e->t.str[1] == '_')){
	k = 1;
	fprintf(f, "yes\n");
	break;
      }
    }
    e = e->next;
  }
  if(!k)
    fprintf(f, "no\n");
  return d;
}

struct lelement * write_repair_break(FILE* f, struct lelement *e){
  struct lelement *d = e;
  int k = 0;
  fprintf(f, "Repair=breaks : ");
  while(e->t.type != END_OF_FILE){
    if(e->t.type == CARD){
      if((e->t.str[0] == 'R') && (e->t.str[1] == '_')){
	k = 1;
	fprintf(f, "yes\n");
	break;
      }
    }
    e = e->next;
  }
  if(!k)
    fprintf(f, "no\n");
  return d;
}

