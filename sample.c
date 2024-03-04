#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// Token types
typedef enum {
    NUMBER, PLUS, MINUS, MULTIPLY, DIVIDE, LPAREN, RPAREN, END, INVALID
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    double value;
} Token;

// Global variables
Token currentToken;
char *input;
int pos = 0;

// Forward declarations
void getNextToken();
void eat(TokenType tokenType);
double factor();
double term();
double expr();

// Table-driven parsing rules
typedef struct {
    TokenType input;
    TokenType next[8];
} ParseTableEntry;

ParseTableEntry parseTable[] = {
    {LPAREN, {LPAREN, NUMBER, PLUS, MINUS, INVALID}},
    {RPAREN, {RPAREN, END, INVALID}},
    {NUMBER, {MULTIPLY, DIVIDE, RPAREN, END, INVALID}},
    {PLUS, {NUMBER, LPAREN, INVALID}},
    {MINUS, {NUMBER, LPAREN, INVALID}},
    {MULTIPLY, {NUMBER, LPAREN, INVALID}},
    {DIVIDE, {NUMBER, LPAREN, INVALID}},
    {END, {END, INVALID}},
    {INVALID, {INVALID}}
};

// Function to get the next token
void getNextToken() {
    char *endptr;
    while (input[pos] == ' ') {
        pos++;
    }

    if (input[pos] == '\0') {
        currentToken.type = END;
        return;
    } else if (isdigit(input[pos]) || input[pos] == '.') {
        currentToken.type = NUMBER;
        currentToken.value = strtod(input + pos, &endptr);
        pos = endptr - input;
    } else {
        switch (input[pos]) {
            case '+':
                currentToken.type = PLUS;
                break;
            case '-':
                currentToken.type = MINUS;
                break;
            case '*':
                currentToken.type = MULTIPLY;
                break;
            case '/':
                currentToken.type = DIVIDE;
                break;
            case '(':
                currentToken.type = LPAREN;
                break;
            case ')':
                currentToken.type = RPAREN;
                break;
            default:
                currentToken.type = INVALID;
                break;
        }
        pos++;
    }
}

// Function to consume a token
void eat(TokenType tokenType) {
    if (currentToken.type == tokenType) {
        getNextToken();
    } else {
        printf("Syntax error: Unexpected token\n");
        exit(1);
    }
}

// Function to parse a factor
double factor() {
    if (currentToken.type == LPAREN) {
        eat(LPAREN);
        double result = expr();
        eat(RPAREN);
        return result;
    } else if (currentToken.type == NUMBER) {
        double result = currentToken.value;
        eat(NUMBER);
        return result;
    } else {
        printf("Syntax error: Invalid factor\n");
        exit(1);
    }
}

// Function to parse a term
double term() {
    double result = factor();
    while (currentToken.type == MULTIPLY || currentToken.type == DIVIDE) {
        if (currentToken.type == MULTIPLY) {
            eat(MULTIPLY);
            result *= factor();
        } else if (currentToken.type == DIVIDE) {
            eat(DIVIDE);
            double divisor = factor();
            if (divisor == 0) {
                printf("Syntax error: Division by zero\n");
                exit(1);
            }
            result /= divisor;
        }
    }
    return result;
}

// Function to parse an expression
double expr() {
    double result = term();
    while (currentToken.type == PLUS || currentToken.type == MINUS) {
        if (currentToken.type == PLUS) {
            eat(PLUS);
            result += term();
        } else if (currentToken.type == MINUS) {
            eat(MINUS);
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
    
    getNextToken(); // Get the first token
    
    double result = expr(); // Parse the expression
    
    if (currentToken.type != END) {
        printf("Syntax error: Invalid expression\n");
        return 1;
    }
    
    printf("Result: %.2f\n", result);
    
    free(input);
    
    return 0;
}
