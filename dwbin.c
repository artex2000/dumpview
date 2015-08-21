#include <stdio.h>
#include <stdlib.h>

#include "dw.h"

static int filesize, curr = 0;
static unsigned char *filebuf;

int getbin (char *name)
{
    FILE *f;
    int fs;

    f = fopen (name, "r");
    if (!f) {
        perror (name);
        return -1;
    }
    
    fseek (f, 0, SEEK_END);
    filesize = ftell (f);
    rewind (f);
    filebuf = (unsigned char *)malloc (filesize * sizeof (char));
    if (!filebuf) {
        perror ("Out of memory");
        return -1;
    }

    fs = fread (filebuf, sizeof (char), filesize, f);
    fclose (f);
    if (fs != filesize) {
        perror ("Error reading binfile");
        free (filebuf);
        return -1;
    }
    return 0;
}

unsigned char *getptr (struct block *b, struct item *i, int *size)
{
    int st = curr;

    if (curr >= filesize || !i || !size)
        return NULL;

    if (i->size == 999) {
        if (b)
            *size = get_block_tail (b);
        else
            printf ("Line %d - out of block scope\n", i->lineno), return NULL;
    } else if (i->size == 9999 || i->size > (filesize - curr)) {
        *size = filesize - curr;
    } else {
        *size = i->size;
    }

    curr += *size;
    return &filebuf[st];
}
