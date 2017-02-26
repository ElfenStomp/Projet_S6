#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token_list.h"


/*
  Sentinel
 */
static const struct lelement sentinel_ar = {{NONE,"void"},(struct lelement*) &sentinel_ar};
#define SENTINEL_AR ((struct lelement*) &sentinel_ar)


/*
  Test equality between two tokens
  returns 1 if t1==t2, 0 oherwise
 */
int equals(struct token t1, struct token t2){
  return ((t1.type == t2.type) && (!strcmp(t1.str,t2.str)));
}

/*
  Creates an empty token list
 */
struct token_list* empty_list() {
  struct token_list* l = malloc(sizeof(*l));
  l->head = SENTINEL_AR;
  return l;
}

/*
  Returns 1 if the list is empty, 0 otherwise
 */

int is_empty (struct token_list* l){
  return (l->head == SENTINEL_AR);
}

/*
  removes head of list and returns it
 */

struct lelement* remove_head(struct token_list* l){
  if (is_empty(l))
    return NULL; //l->head;
  struct lelement* e = l->head;
  l->head = l->head->next;
  e->next = NULL; //a enlever
  return e;
}

/*
  Adds a token at the end of the list
  Returns ADD_SUCCESS if successfull, ADD_FAILURE otherwise
 */

int add_token(struct token_list* l, struct token tok){
  if (is_empty(l)){
    struct lelement* e = malloc(sizeof(*e));
    if(e == NULL)
      return ADD_FAILURE;
    e->t = tok;
    e->next = SENTINEL_AR;
    l->head = e;
    return ADD_SUCCESS;
  }
  struct lelement* f = l->head;
  struct lelement* g = malloc(sizeof(*g));;
  if(g == NULL)
    return ADD_FAILURE;
  while (f->next != SENTINEL_AR){
    f = f->next;
  }
  g->t = tok;
  g->next = SENTINEL_AR;
  f->next = g;
  return ADD_SUCCESS;
}

/*
  Prints the entire token list on screen
 */
void display_list(struct token_list* l){
  struct lelement* e = l->head;
  while (e != SENTINEL_AR){
    printf("TYPE: %s\tVALUE: %s\n", string_from_token_type(e->t.type),e->t.str);
    e = e->next;
  }
}

/*
  Returns a list with all the tokens of a given type
 */
struct token_list* find_token_by_type(struct token_list *l, enum token_type ty){
  struct lelement* e = l->head;
  struct token_list *m = empty_list();
  while (e != SENTINEL_AR){
    if (e->t.type == ty)
      add_token(m,e->t);
    e = e->next;
  }
  return m;
}

/*
  Returns a list with all the tokens except the ones of a given type
 */

struct token_list* remove_token_by_type(struct token_list *l, enum token_type ty){
  struct lelement* e = l->head;
  struct token_list *m = empty_list();
  while (e != SENTINEL_AR){
    if (e->t.type != ty)
      add_token(m,e->t);
    e = e->next;
  }
  return m;
}


/*
  Returns 1 if tok is in l, 0 otherwise
 */
int find_token(struct token_list* l, struct token tok){
  if (is_empty(l))
    return 0;
  struct lelement* e = l->head;
  while (e != SENTINEL_AR){
    if (equals(e->t,tok))
      return 1;
    e = e->next;
  }
  return 0;
}

/*
  frees allocated memory
 */

void free_list(struct token_list *l){
  struct lelement *e = l->head;
  if (!is_empty(l)){
    while (e != SENTINEL_AR){
      free(e);
      e = e->next;
    }
  } 
  free(l);
}

