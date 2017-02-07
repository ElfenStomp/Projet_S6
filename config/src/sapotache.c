#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 
/*
 * I didn't used :
 *      void lexer(FILE * f, struct token * tok);
 *      struct token;
 *      enum token_type;
 *
 * The work TO DO :
 *      -> check if the file is correct before use 'parse' function
 *      -> create other sapo*.["ok","err"].txt files and make differents tests
 */

#define TOKEN_SIZE 256

/*
    To be able to return 2 numbers
*/
struct coord {
    int x;
    int y;
};
/*
    Find board config
 */
struct coord board_config(FILE * f, FILE * out){
    rewind(f);
    struct coord coord;
    
    int i = 0;
    char c = 0;
    char chain[TOKEN_SIZE] = "";
    do {
        c = fgetc(f);
        i = atoi(&c); // seek first carac. which is a number
        if(i > 0){
            fseek(f, -1, SEEK_CUR); // replace cursor
            fscanf(f, "%d %d",&coord.y, &coord.x);
            break;
        }
    } while(fgets(chain, TOKEN_SIZE, f) != NULL);
    fprintf(out, "Configuration : %dx%d\n",coord.x,coord.y);
    return coord;
}

/*
    Count card types and Count cards
 */
void card_types(FILE * f, FILE * out){
    rewind(f);
    int count = 0;
    int nb_card = 0;
    char c;
    char n;
    char chain[TOKEN_SIZE] = "";
    do {
        c = fgetc(f);
        if (c >= 'A' && c <= 'Z'){
            count++;
            while(fgetc(f) != '\n'){}
            fseek(f,-2,SEEK_CUR);
            n = fgetc(f);
            nb_card += atoi(&n);
        }
    } while(fgets(chain, TOKEN_SIZE, f) != NULL);
    fprintf(out,"Card types    : %d\n",count);
    fprintf(out,"Nb of cards   : %d\n",nb_card);
}

/*
    Count and give objectives and holes coordinates
 */
void get_objectives_holes(FILE * f, FILE * out, struct coord coord){
    rewind(f);
    int nbStars = 0;
    int nbArrows = 0;
    int nbDollars = 0;
    int nbPercent = 0;
    
    int *rowDollars = malloc(sizeof(int));
    int *colDollars = malloc(sizeof(int));
    int *rowPercent = malloc(sizeof(int));
    int *colPercent = malloc(sizeof(int));
    int col = 0;
    int row = coord.x-1;
    
    char c;
    char chain[TOKEN_SIZE+100] = "";
    do {
        c = fgetc(f);
        col = 0;
        if (!(c >= 'A' && c <= 'Z') && !(c >= '0' && c <= '9') && !(c == '#') && (c != '\n') && (c != ' ')){
            while(c != '\n' && col < coord.y){
                if(c == '*'){
                    nbStars++;
                } else if(c == '>'){
                    nbArrows++;
                } else if(c == '%'){
                    nbPercent++;
                    rowPercent = realloc(rowPercent, nbPercent*sizeof(int));
                    colPercent = realloc(colPercent, nbPercent*sizeof(int));
                    rowPercent[nbPercent - 1] = row;
                    colPercent[nbPercent - 1] = col;
                } else if(c == '$'){
                    nbDollars++;
                    rowDollars = realloc(rowDollars, nbDollars*sizeof(int));
                    colDollars = realloc(colDollars, nbDollars*sizeof(int));
                    rowDollars[nbDollars - 1] = row;
                    colDollars[nbDollars - 1] = col;
                }
                
                col++;
                c = fgetc(f);
            }
            fseek(f, -1, SEEK_CUR);
            row--;
        }
    } while(fgets(chain, TOKEN_SIZE+100, f) != NULL && row >= 0);
    fprintf(out, "Objectives    : %d - ", nbDollars);
    for (int i = 0; i < nbDollars; i++){
        fprintf(out, "(%d,%d) ", colDollars[i], rowDollars[i]);
    }
    fprintf(out, "\nHoles         : %d - ", nbPercent);
    for (int i = 0; i < nbPercent; i++){
        fprintf(out, "(%d,%d) ", colPercent[i], rowPercent[i]);
    }
    fprintf(out, "\n");
    free(rowPercent);
    free(colPercent);
    free(rowDollars);
    free(colDollars);
}

/*
    Check if Boulder or Break card is authorized
 */
void check_boulder_breaks(FILE * f, FILE * out){
    rewind(f);
    int check[2] = {0, 0};
    char c;
    char chain[TOKEN_SIZE] = "";
    do {
        c = fgetc(f);
        if (c == 'B'){
            c = fgetc(f);
            if (c == 'O'){
                check[0] = 1;
            } else if (c == '_'){
                check[1] = 1;
            }
        }
    } while(fgets(chain, TOKEN_SIZE, f) != NULL);
    fprintf(out, "Allow boulder : %s\n",(check[0] == 1)?"yes":"no");
    fprintf(out, "Allow breaks  : %s\n",(check[1] == 1)?"yes":"no");
}

/*
    Compare the number of Break cards with Repair cards
 */
void check_nb_break(FILE * f, FILE * out){
    rewind(f);
    int nbBreak = 0;
    int nbRepair = 0;
    char c;
    
    char chain[TOKEN_SIZE] = "";
    do {
        c = fgetc(f);
        if (c == 'B'){
            c = fgetc(f);
            if (c == '_'){
                nbBreak++;
            }
        } else if (c == 'R'){
            c = fgetc(f);
            if (c == '_'){
                nbRepair++;
            }
        }
    } while(fgets(chain, TOKEN_SIZE, f) != NULL);
    
    fprintf(out, "Repair=Break  : %s", (nbBreak == nbRepair)?"yes":"no");
}

/*
    Parse a Sapotache file
    PRECOND: f has the expected structure !!!
 */
void parse(FILE * f, FILE * out){
    struct coord coord = board_config(f, out);
    card_types(f, out);
    get_objectives_holes(f, out, coord);
    check_boulder_breaks(f, out);
    check_nb_break(f, out);
}

/*
    Usage function
 */
void usage(char * execname) {
    fprintf(stderr, "Usage: %s <filename>\n", execname);
    fprintf(stderr, "   where <filename> is the file to parse\n");
}


/*
 * Main function
 */
int main(int argc, char * argv[]) {
    if (argc != 2) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }
    
    FILE * f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("*** %s: cannot open file %s\n", argv[0], argv[1]);
        return EXIT_FAILURE;
    }
    
    char *name = argv[1];
    name[14] = 's';
    FILE * out = fopen(name, "w");
    if (out == NULL) {
        printf("*** %s: cannot open file %s\n", argv[0], argv[1]);
        return EXIT_FAILURE;
    }
    
    parse(f, out);
    
    fclose(out);
    fclose(f);
    
    printf("====> Results saved in %s <====\n", name);
    
    return EXIT_SUCCESS;
}
