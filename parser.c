// parser.c

#include <stdio.h>
#include <stdlib.h>
// #include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "scanner.c"

// Global variables
char *input;
int pos = 0;

// Forward declarations
void getNextToken2();
void expr();
// void eat(token tokenType);

void getNextToken2() {
    char *endptr;
    
    scan(input, pos);
    printf("Tyep is: %s\n", names[currentToken.token_type]);
    pos = currentToken.pos;
    printf("next P is %d\n", pos);
    printf("-------------------\n");

}

// Function to parse an expression
void expr() {
    while (currentToken.token_type != eof) {
        getNextToken2();
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
    
    input = (char *)malloc(file_size + 1);
    fread(input, 1, file_size, fp);
    input[file_size] = '\0';
    
    fclose(fp);

    expr();
    
    free(input);

    return 0;
}
