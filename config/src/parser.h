#ifndef _PARSER_H_
#define _PARSER_H_

#define FILE_OK 1
#define FILE_CORRUPT -666

struct token_list* build_token_list(FILE*);
int check_if_valid(FILE*);

#endif //_PARSER_H_
