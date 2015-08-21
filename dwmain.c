#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "dw.h"

int main (int argc, char **argv)
{
    int c, index;
    char *tname;

    while ((c = getopt (argc, argv, "t:")) != -1) {
        switch (c) {
            case 't':
                tname = optarg;
                break;
            default:
                usage ();
                exit (1);
        }
    }
    if (optind == argc) {
        usage ();
        exit (1);
    }

    bname = *argv;
    if (getbin (bname)) {
        perror (bname);
        exit (1);
    }
    
    if (!(yyin = fopen (tname, "r"))) {
        perror (tname);
        exit (1);
    }
    return yyparse();
}

void usage (void)
{
    printf ("Usage: dw -t template binfile\n");
}
