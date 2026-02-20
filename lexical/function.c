#include "main.h"
int is_data_keyword(const char *word)
{
    for (int i = 0; res_kwords_data[i]!= NULL; i++)
        if (strcmp(word, res_kwords_data[i]) == 0)
            return 1;
    return 0;
}

int is_non_data_keyword(const char *word)
{
    for (int i = 0; res_kwords_non_data[i][0]; i++)
        if (strcmp(word, res_kwords_non_data[i]) == 0)
            return 1;
    return 0;
}

int is_operator(char ch)
{
    for (int i = 0; i < sizeof(operators); i++)
        if (operators[i] == ch)
            return 1;
    return 0;
}

int is_symbol(char ch)
{
    for (int i = 0; i < sizeof(symbols); i++)
        if (symbols[i] == ch)
            return 1;
    return 0;
}

/* ---------------- Token Classification ---------------- */

void classify_word(char *word, int line)
{
    int i,dot = 0;
    if (is_data_keyword(word)) {
        printf("[Line %d] DATA KEYWORD       : %s\n", line, word);
        return;
    }

    if (is_non_data_keyword(word)) {
        printf("[Line %d] CONTROL KEYWORD    : %s\n", line, word);
        return;
    }

    /* Hexadecimal */
    if (word[0] == '0' && (word[1] == 'x' || word[1] == 'X')) {
        for (i = 2; word[i]; i++)
            if (!isxdigit(word[i])) {
                printf("[Line %d] ERROR : Invalid hexadecimal %s\n", line, word);
                return;
            }
        printf("[Line %d] HEXADECIMAL CONSTANT : %s\n", line, word);
        return;
    }

    /* Numbers (int / float / octal) */
    if (isdigit(word[0])) {
        for (i = 0; word[i]; i++) {
            if (word[i] == '.')
                dot++;
            else if (!isdigit(word[i])) {
                printf("[Line %d] ERROR : Invalid number %s\n", line, word);
                return;
            }
        }

        if (dot == 0)
            printf("[Line %d] INTEGER CONSTANT : %s\n", line, word);
        else if (dot == 1)
            printf("[Line %d] FLOAT CONSTANT   : %s\n", line, word);
        else
            printf("[Line %d] ERROR : Invalid number %s\n", line, word);

        return;
    }

    /* Identifier */
    if (isalpha(word[0]) || word[0] == '_') {
        if (strlen(word) > 31)
            printf("[Line %d] WARNING : Identifier too long %s\n", line, word);
        else
            printf("[Line %d] IDENTIFIER        : %s\n", line, word);
        return;
    }

    printf("[Line %d] ERROR : Invalid token %s\n", line, word);
}
