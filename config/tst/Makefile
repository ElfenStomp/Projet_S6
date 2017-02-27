CC=gcc
CFLAGS= -std=c99 -Wall -Werror -Wextra
EXEC= test clean
DEBUG= -g -O0
SRC= ../src/token_list.c ../src/parser.c ../src/fonctions_parser.c test_struct_list.c test_parser.c
OBJ=$(SRC:.c=.o) 

all: $(EXEC)

test: $(OBJ)
	@mv ../src/*.o $(PWD)
	$(CC) $(CFLAGS) token_list.o test_struct_list.o -o test_struct_list
	$(CC) $(CFLAGS) token_list.o  parser.o fonctions_parser.o test_parser.o -o test_parser

%.o : %.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(DEBUG)

clean:
	@rm -f *.o
