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
bool debug = false;
// Forward declarations
void readNext();
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
    printf("Syntax error at position %s\n", &stack[returnData.prev_pos]);
    exit(1);
}

/*---------------------Parse Table----------------------*/
void stmt() {
    readNext();
    if (returnData.token_type == id) {
        readNext();
        if (returnData.token_type != lineBreak &&
            returnData.token_type != eof) {error();}
    } else if (returnData.token_type == becomes) {
        readNext();
        expr();
    }
}

void stmt_list() {
    switch (returnData.token_type) {
        case id:
            stmt();
            return;
        case read:
            stmt();
            return;
        case write:
            stmt();
            return;
        case SC:
            while (returnData.token_type != lineBreak){
                readNext();
            }
            return;
        case OC:
            while (returnData.token_type != CC){
                readNext();
            }
            readNext();
            return;
        default:
            error();

    }
}

// Factor :: = '(' Expr ')' | number
void factor() {
    // printf("factor type is----> %s\n", names[returnData.token_type]);
    if (returnData.token_type == lparen) {
        readNext();
        expr();
        if (returnData.token_type == rparen) {
            readNext();
        } else {
            error();
        }
    } else if (returnData.token_type == id || 
        returnData.token_type == literal) {
        readNext();
    } else {
        error();
    }
}

void factor_tail() {
    if (returnData.token_type == mul || 
        returnData.token_type == divide) {
        readNext();
        factor();
        factor_tail();
    }
}

void term() {
    factor();
    factor_tail();
}

void term_tail() {
    if (returnData.token_type == add || 
        returnData.token_type == sub) {
        readNext();
        term();
        term_tail();
    }
}

// Expr :: = Term Term_tail
void expr() {
    term();
    term_tail();
    // printf("-----Exit expr-----");
}


/*------------------Scan and update the pointer (st)------------------*/
void readNext() {    
    scan(stack, pos, open_comment);
    pos = returnData.pos;
    open_comment = returnData.open_comment;
    if (debug == true) {
        printf("Tyep is: %s\n", names[returnData.token_type]);
        printf("next P is %d\n", pos);
        printf("-------------------\n");
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
    readNext();
    while (returnData.token_type != eof ){
        stmt_list();
        readNext();
    }

    printf("Syntax is correct!\n");
    
    free(stack);

    return 0;
}
