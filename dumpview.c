#include <stdio.h>
#include <stdlib.h>
#include "dumpview.h"

int main (int argc, char **argv)
{
    if (argc > 1) {
        if (!(yyin = fopen (argv[1], "r"))) {
            perror (argv[1]);
            return 1;
        }
    } else {
        printf ("usage: %s filename\n", argv[0]);
        return 0;
    }
    yylex();
    return 0;
}
