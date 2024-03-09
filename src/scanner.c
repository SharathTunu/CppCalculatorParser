#include <stdio.h>
#include <string.h>
#include "ctype.h"
#include <stdbool.h>

// Changed div to divide as div is a key word in stdlib library and its creating compilation errors
typedef enum {read, write, id, literal, becomes, lineBreak, OC, CC, SC,
                add, sub, mul, divide, lparen, rparen, eof, lerror} token;

// Token structure
typedef struct {
    token token_type;
    int pos;
    bool open_comment;
    int prev_pos;
} Token;

// Global variables
char token_image[100];

char* names[] = {"read", "write", "id", "literal", "becomes", "lineBreak", "openComent", "closeComent",
                "shortComent", "add", "sub", "mul", "divide", "lparen", "rparen", "eof", "lexical error"};

Token returnData;

void lex_error(char *input_data) {
    printf("Lexical error at position %s\n", &input_data[returnData.prev_pos]);
    exit(1);

}

void scan(char *input_data, int postion, bool open_comment) {
    returnData.open_comment = open_comment;
    returnData.prev_pos = postion;
    printf("current Pointer idx is %d\n", postion);
    int i = 0;              /* index into token_image */
    while (input_data[postion] == ' ') {
        postion++;
    }
    // Check for the end of program
    
    // printf("%s", &input_data[postion]);
    if (input_data[postion] == '\0') {
        printf("EOF\n");
        returnData.token_type = eof;
        returnData.pos = postion;
        return;
    }
    if (input_data[postion] == '\n') {
        printf("EOL\n");
        if (returnData.open_comment == true) {
            lex_error(input_data);
        }
        postion++;
        returnData.token_type = lineBreak;
        returnData.pos = postion;
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
            returnData.token_type = read;
            returnData.pos = postion;
            printf("read\n");
            return;
        }
        else if (!strcmp(token_image, "write")) {
            returnData.token_type = write;
            returnData.pos = postion;
            printf("write\n");
            return;
        }else {
            returnData.token_type = id;
            returnData.pos = postion;
            printf("id\n");
            return;
        }
    
    } else if (isdigit(input_data[postion])) {
        do {
            token_image[i++] = input_data[postion];
            postion++;
        } while (isdigit(input_data[postion]));
        token_image[i] = '\0';
        returnData.token_type = literal;
        returnData.pos = postion;
        printf("literal\n");
        return;
    } else switch (input_data[postion]) {
            case ':':
                postion++;
                if (input_data[postion] != '=') {
                    lex_error(input_data);
                } else {
                    postion++;
                    returnData.token_type = becomes;
                    returnData.pos = postion;
                    printf("becomes\n");
                }
                return;
            case '/':
                postion++;
                if (input_data[postion] == '*') {
                    postion++;
                    returnData.token_type = OC;
                    returnData.pos = postion;
                    returnData.open_comment = true;
                    printf("openComent\n");
                    return;
                }else if (input_data[postion] == '/') {
                    postion++;
                    returnData.token_type = SC;
                    returnData.pos = postion;
                    printf("shortComent\n");
                    return;
                }
                returnData.token_type = divide;
                returnData.pos = postion;
                printf("divide\n");
                return;
            case '*':
                postion++;
                if (input_data[postion] == '/') {
                    postion++;
                    returnData.token_type = CC;
                    returnData.pos = postion;
                    returnData.open_comment = false;
                    printf("closeComent\n");
                    return;
                }
                returnData.token_type = mul;
                returnData.pos = postion;
                printf("multiply\n");
                return;
            case '+':
                postion++;
                returnData.token_type = add;
                returnData.pos = postion;
                printf("add\n");
                return;
            case '-':
                postion++;  
                returnData.token_type = sub;
                returnData.pos = postion;
                printf("sub\n");
                return;
            case '(':
                postion++;
                returnData.token_type = lparen;
                returnData.pos = postion;
                printf("lparen\n");
                return;
            case ')':
                postion++;
                returnData.token_type = rparen;
                returnData.pos = postion;
                printf("rparen\n");
                return;
            default:
                printf("lexical error\n");
                exit(1);
        }
    printf("Nothing doing\n");
}