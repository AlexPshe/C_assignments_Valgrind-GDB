//
// Created by alex_pshe on 21.03.18.
//
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

int main(){
    char *s;
    int ln;
    puts("Enter String");
    gets(s);
    ln = strlen(s);
    char *dyn_s = (char*) malloc(strlen(s)+1);
    dyn_s = s;
    dyn_s[strlen(s)] = '\0';
    puts(dyn_s);
    return 0;
}

