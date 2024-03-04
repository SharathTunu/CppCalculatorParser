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
void getNextToken();
void getNextToken2();
double expr();
// void eat(token tokenType);

// Function to get the next token
void getNextToken() {
    char *endptr;
    while (input[pos] == ' ') {
        pos++;
    }

    if (input[pos] == '\0') {
        printf(" END;");
        return;
    } else if (isdigit(input[pos]) || input[pos] == '.') {
        printf(" NUMBER;");
        // currentToken.value = strtod(input + pos, &endptr);
        pos = endptr - input;
    } else {
        switch (input[pos]) {
            case '+':
                printf(" PLUS;");
                break;
            case '-':
                printf(" MINUS;");
                break;
            case '*':
                printf(" MULTIPLY;");
                break;
            case '/':
                printf(" DIVIDE;");
                break;
            case '(':
                printf(" LPAREN;");
                break;
            case ')':
                printf(" RPAREN;");
                break;
            default:
                printf(" INVALID;");
                break;
        }
        pos++;
    }
}

void getNextToken2() {
    char *endptr;
    
    scan(input, pos);
    printf("Tyep is: %s\n", names[currentToken.token_type]);
    pos = currentToken.pos;
    printf("next P is %d\n", pos);
    printf("-------------------\n");

}

// Function to parse an expression
double expr() {
    double result = term();
    while (currentToken.token_type == add || currentToken.token_type == sub) {
        if (currentToken.token_type == add) {
            eat(add);
            result += term();
        } else if (currentToken.token_type == sub) {
            eat(sub);
            result -= term();
        }
    }
    return result;
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

    getNextToken2();
    getNextToken2();
    getNextToken2();
    getNextToken2();
    // getNextToken2();
    // getNextToken();
    // getNextToken();
    // getNextToken();
    // getNextToken();
    
    // double result = expr(); // Parse the expression
    
    // if (currentToken.type != END) {
    //     printf("Syntax error: Invalid expression\n");
    //     return 1;
    // }
    
    // printf("Result: %.2f\n", result);
    
    free(input);

    return 0;
}
