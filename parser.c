// parser.c

#include <stdio.h>
#include <stdlib.h>
// #include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "scanner.c"

// Global variables
char *stack;
int pos = 0;
bool open_comment = false;
bool debug = true;
// Forward declarations
void match(char predict);
void stmt_list();
void expr();
void factor_tail();
void factor();
void term_tail();
void term();
void expr();
void error();

/*----------------Helper functions------------------*/
void error() {
    // Print errors
    printf("Syntax error at position %s\n", &stack[pos]);
    exit(EXIT_FAILURE);
}

void match(char predict) {
    // Function to match against next predicted character
    if (stack[pos] == predict) {
        pos++;
    } else {
        error();
    }
}

/*---------------------Parse Table----------------------*/

/*------------------Scan and update the pointer (st)------------------*/
void stmt_list() {    
    while (returnData.token_type != eof) {
        scan(stack, pos, open_comment);
        pos = returnData.pos;
        open_comment = returnData.open_comment;
        if (debug == true) {
            printf("Tyep is: %s\n", names[returnData.token_type]);
            printf("next P is %d\n", pos);
            printf("-------------------\n");
        }
    }
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Error opening file: %s\n", argv[1]);
        return 1;
    }
    
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    stack = (char *)malloc(file_size + 1);
    fread(stack, 1, file_size, fp);
    stack[file_size] = '\0';
    
    fclose(fp);

    // Init parsing
    stmt_list();
    
    free(stack);

    return 0;
}
