#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define WORD_BUFF_SIZE 64

int is_data_keyword(const char *);
int is_non_data_keyword(const char *);
int is_operator(char);
int is_symbol(char);
void classify_word(char *, int);
/* ---------------- Reserved Keywords ---------------- */

static char *res_kwords_data[] = {
    "const","volatile","extern","auto","register",
    "static","signed","unsigned","short","long",
    "double","char","int","float","struct",
    "union","enum","void","typedef",NULL
};

static char *res_kwords_non_data[] = {
    "goto","return","continue","break",
    "if","else","for","while","do",
    "switch","case","default","sizeof",""
};

/* ---------------- Operators & Symbols ---------------- */

static char operators[] = {
    '+','-','*','/','%','=','<','>','!','&','|','^','~'
};

static char symbols[] = {
    '(',')','{','}','[',']',';',',','"'
};