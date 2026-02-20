#include "main.h"

int main(int argc, char *argv[])
{
    FILE *fp;
    char ch, word[WORD_BUFF_SIZE];
    int word_idx = 0;
    int line = 1;

    if (argc != 2) {
        printf("Usage: ./a.out <file.c>\n");
        return 0;
    }

    fp = fopen(argv[1], "r");
    if (!fp) {
        printf("Error: file Cannot open\n");
        return 0;
    }

    while ((ch = fgetc(fp)) != EOF) {

        if (ch == '\n')
            line++;
        
        /* Header file */
        if (ch == '#') {
            fscanf(fp, "include<%[^>]>", word);
            printf("[Line %d] HEADER FILE       : %s\n", line, word);
            continue;
        }

        /* Build words */
        if (isalnum(ch) || ch == '_' || ch == '.') {
            word[word_idx++] = ch;
        }
        else {
            /* Finalize word */
            if (word_idx > 0) {
                word[word_idx] = '\0';
                classify_word(word, line);
                word_idx = 0;
            }

            /* Operators */
            if (is_operator(ch)) {
                printf("[Line %d] OPERATOR           : %c\n", line, ch);
            }

            /* Symbols */
            else if (is_symbol(ch)) {
                printf("[Line %d] SYMBOL             : %c\n", line, ch);
            }

            /* Unknown characters */
            else if (!isspace(ch)) {
                printf("[Line %d] ERROR              : Unknown symbol %c\n", line, ch);
            }
        }
    }
    

    fclose(fp);
    return 0;

}