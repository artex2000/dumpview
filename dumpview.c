#include <stdio.h>
#include <stdlib.h>
#include "dumpview.h"
#include "dumpview.tab.h"

extern int yydebug;

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
    yydebug = 1;
    yyparse();
    return 0;
}

void process (struct item_list *l)
{
    struct item *i;

    while (l) {
        i = l->i;
        switch (i->t) {
            case 0:
                print_n (i);
                break;
            case 1:
                print_v ((struct vitem *)i);
                break;
            case 2:
                print_b ((struct bitem *)i);
                break;
            default:
                break;
        }
        l = l->next;
    }
}

void release (struct item_list *l)
{
    struct item *i;
    struct item_list *s;

    while (l) {
        i = l->i;
        if (i->t == 2)
            release (((struct bitem *)i)->c);
        s = l->next;
        free (l);
        l = s;
    }
}

struct item *newitem (char *n)
{
    struct item *i;

    i = malloc (sizeof (struct item));
    if (i == NULL) yyerror ("Not enough memory");
    i->t = 0;
    i->n = n;
    return i;
}

struct item *newvitem (char *n, char f, int s)
{
    struct vitem *i;

    i = malloc (sizeof (struct vitem));
    if (i == NULL) yyerror ("Not enough memory");
    i->i.t = 1;
    i->i.n = n;
    i->f = f;
    i->s = s;
    return (struct item *)i;
}

struct item *newbitem (char *n, int r, struct item_list *c)
{
    struct bitem *i;

    i = malloc (sizeof (struct bitem));
    if (i == NULL) yyerror ("Not enough memory");
    i->i.t = 2;
    i->i.n = n;
    i->r = r;
    i->c = c;
    return (struct item *)i;
}

struct item_list *newlist (struct item *i, struct item_list *s)
{
    struct item_list *l;

    l = malloc (sizeof (struct item_list));
    if (l == NULL) yyerror ("Not enough memory");
    l->i = i;
    l->next = s;
    return l;
}

void print_n (struct item *i)
{
    printf ("name %s", i->n);
}

void print_v (struct vitem *i)
{
    printf ("name %s, format %c, size %d\n", i->i.n, i->f, i->s);
}

void print_b (struct bitem *i)
{
    struct item_list *s;
    printf ("name %s, rep %d\n", i->i.n, i->r);
    s = i->c;
    while (s) {
        switch (s->i->t) {
            case 0:
                print_n (s->i);
                break;
            case 1:
                print_v ((struct vitem *)s->i);
                break;
            case 2:
                print_b ((struct bitem *)s->i);
                break;
        }
        s = s->next;
    }
}
