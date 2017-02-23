#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/token_list.h"

int test_passed = 0;

void assertp(int bool){
  assert(bool);
  printf(".");
  fflush(stdout);
  test_passed++;
}

void tst_is_empty(){
  struct token_list* l = empty_list();
  assertp(is_empty(l));
  free_list(l);
}

void tst_add_token(){
  struct token_list* l = empty_list();
  struct token tok1 = {NONE,"tok1"};
  struct token tok2 = {NONE,"tok2"};
  assertp(add_token(l,tok1) == ADD_SUCCESS);
  assertp(!strcmp("tok1",l->head->t.str));
  assertp(add_token(l,tok2) == ADD_SUCCESS);
  free(l);
}

void tst_find_token(){
  struct token_list* l = empty_list();
  struct token tok1 = {NONE,"tok1"};
  struct token tok2 = {NONE,"tok2"};  
  assertp(!(find_token(l,tok1)));
  add_token(l,tok1);
  add_token(l,tok2);
  assertp(find_token(l,tok1));
  assertp(find_token(l,tok2));
  free(l);
}

void tst_remove_head(){
  struct token_list* l = empty_list();
  struct token tok1 = {NONE,"tok1"};
  struct token tok2 = {NUMBER,"tok2"};  
  add_token(l,tok1);
  add_token(l,tok2);
  struct lelement* e = remove_head(l);
  assertp(e->t.type == NONE);
  assertp(l->head->t.type == NUMBER);
  free(l);
}

void tst_find_token_by_type(){
  struct token_list* l = empty_list();
  struct token tok1 = {NONE,"tok1"};
  struct token tok2 = {NUMBER,"tok2"};
  struct token tok3 = {ID,"tok3"};
  struct token tok4 = {NONE,"tok4"};
  
  add_token(l,tok1);
  add_token(l,tok2);
  add_token(l,tok3);
  add_token(l,tok4);

  struct token_list* m = find_token_by_type(l,NUMBER);
  assertp(m->head->t.type == NUMBER);
  free(l);
}

void tst_remove_token_by_type(){
  struct token_list* l = empty_list();
  struct token tok1 = {NONE,"tok1"};
  struct token tok2 = {NUMBER,"tok2"};
  struct token tok3 = {ID,"tok3"};
  struct token tok4 = {NONE,"tok4"};
  
  add_token(l,tok1);
  add_token(l,tok2);
  add_token(l,tok3);
  add_token(l,tok4);

  struct token_list* m = remove_token_by_type(l,NONE);
  assertp(!(is_empty(m)));
  assertp(m->head->t.type == NUMBER);
  assertp(m->head->next->t.type == ID);
  free(l);
}

int main(){
  tst_is_empty();
  tst_add_token();
  tst_find_token();
  tst_remove_head();
  tst_find_token_by_type();
  tst_remove_token_by_type();
  
  printf("Structural tests passed : %d\n",test_passed);
  return EXIT_SUCCESS;
}
