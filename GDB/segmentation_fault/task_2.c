//
// Created by alex_pshe on 21.03.18.
//
# include <stdlib.h>
# include <stdio.h>

int main()
{
    char *a = (char*) malloc(6 * sizeof(char));
    char *str = "Hello";
    *a = *str;
    printf ("%s\n", *a);
    free(a);
    return 0;
}

