#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include "../src/token_list.h"
#include "../src/parser.h"

/*
int test_passed = 0;

void assertp(int bool){
  assert(bool);
  printf(".");
  fflush(stdout);
  test_passed++;
}
*/

/*
  Description
*/
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

/*
  Description
*/
void tst_check_if_valid(char* file){
  FILE* f = fopen(file, "r");
  if (f == NULL){
    printf("ERROR: File %s doesn't exist\n", file);
    return;
  }

  //assertp(check_if_valid(f) == FILE_OK);
  if(check_if_valid(f)==1)
    printf("%s ...FILE_OK\n",file);
  else
    printf("%s ...FILE_CORRUPT\n",file);
}

// TO DO (camille): 
// - Test tout si pas de paramètres sinon test le fichier en paramètre 
// - s'assurer que le fichier saisie existe / virer le code redondant
// - Messages de description
// - vérifier tst_build_token_list 
int main(int argc, char* argv[]){
  printf("%s", argv[0]);
  if (argc != 1) {
    printf("Usage ./test_parser\n");
    return EXIT_FAILURE;
  }
  
  DIR *d;
  struct dirent *dir;
  d = opendir(".");

  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (strstr(dir->d_name, "sapo") != NULL && strstr(dir->d_name, "txt") != NULL) {
	tst_check_if_valid(dir->d_name);
  	//tst_build_token_list(dir->d_name);
      }
    }
    closedir(d);
  }

  return EXIT_SUCCESS;
}










