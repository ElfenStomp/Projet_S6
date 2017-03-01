#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/token_list.h"
#define NB_TEST 20

int test_passed = 0;

void assertp(int bool){
  assert(bool);
  printf(".");
  fflush(stdout);
  test_passed++;
}

void tst_is_empty(){
  struct token_list* l = empty_list();
  struct token tok = {NONE,"tok"};
  assertp(is_empty(l));
  add_token(l,tok);
  assertp(! is_empty(l));
  free_list(l);
}

void tst_add_token(){
  struct token_list* l = empty_list();
  struct token tok1 = {NONE,"tok1"};
  struct token tok2 = {NONE,"tok2"};
  assertp(add_token(l,tok1) == ADD_SUCCESS);
  assertp(!strcmp("tok1",l->head->t.str));
  assertp(add_token(l,tok2) == ADD_SUCCESS);
  assertp(!strcmp("tok2",l->head->next->t.str));
  free_list(l);
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
  free_list(l);
}

void tst_remove_head(){
  struct token_list* l = empty_list();
  assertp(!(strcmp(remove_head(l).str,"ERR_EMPTY_LIST")));
  struct token tok1 = {NONE,"tok1"};
  struct token tok2 = {NUMBER,"tok2"};  
  add_token(l,tok1);
  add_token(l,tok2);
  struct token t = remove_head(l);
  assertp(t.type == NONE);
  assertp(l->head->t.type == NUMBER);
  free_list(l);
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

  struct token_list* m1 = find_token_by_type(l,NUMBER);
  assertp(m1->head->t.type == NUMBER);
  struct token_list* m2 = find_token_by_type(l,STAR);
  assertp(is_empty(m2));
  struct token_list* m3 = find_token_by_type(l,NONE);
  assertp(m3->head->t.type == NONE);
  assertp(m3->head->next->t.type == NONE);
  free_list(l);
  free_list(m1);
  free_list(m2);
  free_list(m3);
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

  struct token_list* m1 = remove_token_by_type(l,NONE);
  assertp(!(is_empty(m1)));
  assertp(m1->head->t.type == NUMBER);
  assertp(m1->head->next->t.type == ID);
  struct token_list* m2 = find_token_by_type(l,NONE);
  struct token_list* m3 = remove_token_by_type(m2, NONE);
  assertp(is_empty(m3));
  free_list(l);
  free_list(m1);
  free_list(m2);
  free_list(m3);
}

int main(){
  tst_is_empty();
  tst_add_token();
  tst_find_token();
  tst_remove_head();
  tst_find_token_by_type();
  tst_remove_token_by_type();
  
  printf("Structural tests passed : %d/%d\n", test_passed,NB_TEST);
  return EXIT_SUCCESS;
}
