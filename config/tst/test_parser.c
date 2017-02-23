#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "../src/token_list.h"

#include "../src/parser.h"

int test_passed = 0;

void assertp(int bool){
  assert(bool);
  printf(".");
  fflush(stdout);
  test_passed++;
}

void tst_build_token_list(char* file){
  FILE* f = fopen(file, "r");
  if (f == NULL){
    printf("ERROR: File %s doesn't exist\n", file);
    return;
  }
  struct token_list* l = build_token_list(f);
  l = remove_token_by_type(l,COMMENT);
  display_list(l);
  fclose(f);
  
}

void tst_check_if_valid(char* file){
  FILE* f = fopen(file, "r");
  if (f == NULL){
    printf("ERROR: File %s doesn't exist\n", file);
    return;
  }
  //assertp(check_if_valid(f) == FILE_OK);
  printf("%d\n",check_if_valid(f));
}

int main(int argc, char* argv[]){
  if (argc != 2){
    printf("Usage ./test_parser <test_file.txt>\n");
    return EXIT_FAILURE;
  }
  //tst_build_token_list(argv[1]);
  tst_check_if_valid(argv[1]);
  return EXIT_SUCCESS;
}
