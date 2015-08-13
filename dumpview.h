#ifndef __DUMP_VIEW_H_
#define  __DUMP_VIEW_H_

extern int yylineno;
extern FILE *yyin;

void yyerror (char *s, ...);
int yylex (void);

struct item {
    int t;      /*node type*/
    char *n;    /*node name*/
};

struct vitem {
    struct item i;
    char f;     /* node format */ 
    int s;      /* node size */
};

struct bitem {
    struct item i;
    int r;      /* repeat counter */
    struct item_list *c;    /* child list */
};

struct item_list {
    struct item *i;        /* item */
    struct item_list *next;
};

struct item *newitem (char *nm);
struct item *newvitem (char *nm, char fo, int sz);
struct item *newbitem (char *nm, int rp, struct item_list *s);
struct item_list *newlist (struct item *i, struct item_list *s);

void print_n (struct item *i);
void print_v (struct vitem *i);
void print_b (struct bitem *i);
#endif

