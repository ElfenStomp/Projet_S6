#ifndef _TOKEN_LIST_H_
#define _TOKEN_LIST_H_

#define ADD_SUCCESS 0
#define ADD_FAILURE 1
#define INVALID_FILE -666

enum token_type {
  NONE,
  COMMENT,
  SPACE,
  CARD, 
  ID,
  NUMBER,
  STAR,
  ARROW,
  DOLLAR,
  PERCENT,
  RETURN,
  END_OF_FILE,
};

static inline char *string_from_token_type(enum token_type t){
  static char* str[] = {"NONE","COMMENT","SPACE","CARD","ID","NUMBER","STAR","ARROW","DOLLAR","PERCENT","RETURN","END_OF_FILE"};
  return str[t];
}

#define TOKEN_SIZE 256


struct token {
  enum token_type type;
  char str[TOKEN_SIZE];
};

/*
  Definition of a linked chain containing tokens 
 */
struct lelement {
  struct token t;
  struct lelement* next;
};

struct token_list {
  struct lelement* head;
};

///////////////////////////////
//         FUNCTIONS         //
///////////////////////////////

//creates an empty list
struct token_list* empty_list();
//returns 1 if list is empty, 0 otherwise
int is_empty (const struct token_list*);
//removes the head of the list and returns it
struct lelement* remove_head(struct token_list*);
//adds a token at the end of the list
int add_token(struct token_list*, struct token);
//returns 1 if the token is in the list, 0 otherwise
int find_token(struct token_list*, struct token);
//prints the entire list on screen
void display_list(struct token_list*);
//frees allocated memory
void free_list(struct token_list*);
//returns a list containing all tokens of a given type in the parameter list
struct token_list* find_token_by_type(struct token_list*, enum token_type);
//returns a list containing all BUT tokens of a given type in the parameter list
struct token_list* remove_token_by_type(struct token_list*, enum token_type);

#endif // _TOKEN_LIST_H_
