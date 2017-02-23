CFLAGS= -std=c99 -Wall -Werror -Wextra
DEBUG= -g -O0
OBJFILES= token_list.o test_struct_list.o parser.o test_parser.o

test:	$(OBJFILES)
	gcc $(CFLAGS) token_list.o test_struct_list.o -o test_struct_list
	gcc $(CFLAGS) token_list.o  parser.o test_parser.o -o test_parser

token_list.o:
	gcc $(CFLAGS) -c $(DEBUG) ../src/token_list.c

test_struct_list.o:
	gcc $(CFLAGS) -c $(DEBUG) test_struct_list.c

test_parser.o:
	gcc $(CFLAGS) -c $(DEBUG) test_parser.c

parser.o:
	gcc $(CFLAGS) -c $(DEBUG) ../src/parser.c

clean:
	rm -f *.o *~
