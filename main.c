#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum TokenType
{
    LT,
    GT,
    BAR,
    TAG_ID,
    PROP_ID,
    STRING,
    PROP_VAL,
    EQ,
    QUOTE,
    TEOF
} TokenType;

typedef struct
{
    char *value;
    TokenType type;
} Token;

size_t getinputsize(FILE *);
TokenType get_token_from_char(char, int *);
void read_input(FILE *, char *, size_t);
void get_next_token(char *, Token *, int *);

int main()
{
    FILE *file = fopen("example.html", "r");

    if (file == NULL)
    {
        printf("Not able to open the file.\n");
        return 1;
    }

    size_t input_len = getinputsize(file);

    char *input = malloc((input_len + 1) * sizeof(char));
    read_input(file, input, input_len);

    Token *curr_token = malloc(sizeof(Token));
    int pos = 0;

    while (curr_token->type != TEOF)
    {
        get_next_token(input, curr_token, &pos);
    }

    return 0;
}

void get_next_token(char *input, Token *curr_token, int *pos)
{
    char ch = input[*pos];
    char *alnum_stream = malloc(sizeof(char));

    TokenType tk_t;

    // skip whitespaces
    while (isspace(ch))
    {
        *pos += 1;
        ch = input[*pos];
    }

    // get tag props value
    if (ch == '"')
    {
        tk_t = PROP_VAL;
        while (1)
        {
            *pos += 1;
            if (input[*pos] == '"')
                break;
            ch = input[*pos];
            strcat(alnum_stream, &ch);
        }
    }
    else if (isalnum(ch))
    {
        tk_t = PROP_ID;

        if (input[*pos - 1] == '<' || input[*pos - 1] == '/')
        {
            tk_t = TAG_ID;
        }

        if (input[*pos - 1] == '>')
        {
            tk_t = STRING;
        }

        while (1)
        {
            strcat(alnum_stream, &ch);
            if (!isalnum(input[*pos + 1]))
                break;
            *pos += 1;
            ch = input[*pos];
        }
    }

    if (ch == '<')
        tk_t = LT;
    if (ch == '>')
        tk_t = GT;
    if (ch == '/')
        tk_t = BAR;
    if (ch == '=')
        tk_t = EQ;

    if (ch != 0)
    {
        curr_token->type = tk_t;
        curr_token->value = *alnum_stream == 0 ? &ch : alnum_stream;
        printf("%d: %s\n", curr_token->type, curr_token->value);
        *pos += 1;
        return;
    }

    curr_token->type = TEOF;
    curr_token->value = 0;
}

size_t getinputsize(FILE *file)
{
    fseek(file, 0, SEEK_END);
    int file_len = ftell(file);
    fseek(file, 0, SEEK_SET);
    return (size_t)file_len;
}

void read_input(FILE *file, char input[], size_t input_len)
{
    for (int i = 0; i < input_len; i++)
    {
        input[i] = fgetc(file);
    }
    input[input_len] = '\0';
}