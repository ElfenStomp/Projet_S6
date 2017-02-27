#ifndef _PARSER_H_
#define _PARSER_H_

#define FILE_OK 1
#define FILE_CORRUPT -666

struct token_list* build_token_list(FILE*);
int check_if_valid(FILE*);

void token_comment(FILE*, char, struct token*);
void token_number(FILE*, char, struct token*);
void token_space(FILE*, char, struct token*);
void token_star(struct token*);
void token_return(struct token*);
void token_arrow(struct token*);
void token_dollar(struct token*);
void token_percent(struct token*);
void token_end(struct token*);
void token_card(FILE*, char, struct token*);

int check_card(char*);

#endif //_PARSER_H_
