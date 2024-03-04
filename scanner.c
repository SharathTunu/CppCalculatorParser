#include <stdio.h>
#include <string.h>
#include "ctype.h"

// Changed div to divide as div is a key word in stdlib library and its creating compilation errors
typedef enum {read, write, id, literal, becomes, lineBreak,
                add, sub, mul, divide, lparen, rparen, eof, lerror} token;

// Token structure
typedef struct {
    token token_type;
    double value;
    int pos;
} Token;

// Global variables
char token_image[100];

char* names[] = {"read", "write", "id", "literal", "becomes", "lineBreak",
                "add", "sub", "mul", "divide", "lparen", "rparen", "eof", "lexical error"};

Token currentToken;
void scan(char *input_data, int postion) {
    printf("current P is %d\n", postion);
    int i = 0;              /* index into token_image */
    while (input_data[postion] == ' ') {
        postion++;
    }
    // Check for the end of program
    
    // printf("%s", &input_data[postion]);
    if (input_data[postion] == '\0') {
        printf("EOF\n");
        currentToken.token_type = eof;
        currentToken.pos = postion;
        return;
    }
    if (input_data[postion] == '\n') {
        printf("EOL\n");
        postion++;
        currentToken.token_type = lineBreak;
        currentToken.pos = postion;
        return;
    }
    // Check for read or write
    
    if (isalpha(input_data[postion])) {
        do {
            token_image[i++] = input_data[postion];
            postion++;
        } while (isalpha(input_data[postion]));

        token_image[i] = '\0';
        if (!strcmp(token_image, "read")) {
            currentToken.token_type = read;
            currentToken.pos = postion;
            printf("read\n");
            return;
        }
        else if (!strcmp(token_image, "write")) {
            currentToken.token_type = write;
            currentToken.pos = postion;
            printf("write\n");
            return;
        }else {
            currentToken.token_type = id;
            currentToken.pos = postion;
            printf("id\n");
            return;
        }
    
    } else if (isdigit(input_data[postion])) {
        do {
            token_image[i++] = input_data[postion];
            postion++;
        } while (isdigit(input_data[postion]));
        token_image[i] = '\0';
        currentToken.token_type = literal;
        currentToken.pos = postion;
        printf("literal\n");
        return;
    } else switch (input_data[postion]) {
            case ':':
                postion++;
                if (input_data[postion] != '=') {
                    fprintf(stderr, "lexical error\n");
                    exit(1);
                } else {
                    postion++;
                    currentToken.token_type = becomes;
                    currentToken.pos = postion;
                    printf("becomes\n");
                }
                return;
            case '+':
                postion++;
                currentToken.token_type = add;
                currentToken.pos = postion;
                printf("add\n");
                return;
            case '-':
                postion++;  
                currentToken.token_type = sub;
                currentToken.pos = postion;
                printf("sub\n");
                return;
            case '*':
                postion++;
                currentToken.token_type = mul;
                currentToken.pos = postion;
                printf("multiply\n");
                return;
            case '/':
                postion++;
                currentToken.token_type = divide;
                currentToken.pos = postion;
                printf("divide\n");
                return;
            case '(':
                postion++;
                currentToken.token_type = lparen;
                currentToken.pos = postion;
                printf("lparen\n");
                return;
            case ')':
                postion++;
                currentToken.token_type = rparen;
                currentToken.pos = postion;
                printf("rparen\n");
                return;
            default:
                printf("lexical error\n");
                exit(1);
        }
    printf("Nothing doing\n");
}