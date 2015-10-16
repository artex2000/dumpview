#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dumpview.h"
#include "dumpview.tab.h"

extern int yydebug;
static unsigned char sample[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19 };
static unsigned char *buf = sample;
static int buf_size = 20;
static struct item_list *root;

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
    yydebug = 0;
    yyparse();
    printf ("\n");
    free_item_list (root);
    return 0;
}

void process_item_list (struct item_list *l, struct bl_item *p)
{
    struct item *i;
    if (p == NULL)
        root = l;

    while (l) {
        i = l->i;
        switch (i->type) {
            case SINGLE_LINE:
                sl_print ((struct sl_item *)i, p);
                break;
            case PARAM_LINE:
                pl_print ((struct pl_item *)i, p);
                break;
            case PARAM_F_LINE:
                plf_print ((struct plf_item *)i, p);
                break;
            case BLOCK_LINE:
                bl_print ((struct bl_item *)i, p);
                break;
            default:
                break;
        }
        l = l->next;
    }
}

void free_item_list (struct item_list *l)
{
    struct item *i;
    struct item_list *s;

    while (l) {
        i = l->i;
        switch (i->type) {
            case SINGLE_LINE:
                sl_free ((struct sl_item *)i);
                break;
            case PARAM_LINE:
                pl_free ((struct pl_item *)i);
                break;
            case PARAM_F_LINE:
                plf_free ((struct plf_item *)i);
                break;
            case BLOCK_LINE:
                bl_free ((struct bl_item *)i);
                break;
            default:
                break;
        }
        s = l->next;
        free (l);
        l = s;
    }
}

struct item *new_sl_item (char *n)
{
    struct sl_item *i;

    i = malloc (sizeof (*i));
    if (i == NULL) yyerror ("Not enough memory");
    i->type = SINGLE_LINE;
    i->lineno = yylineno;
    i->name = n;
    return (struct item *)i;
}

struct item *new_pl_item (char *n, struct param_list *p)
{
    struct pl_item *i;

    i = malloc (sizeof (*i));
    if (i == NULL) yyerror ("Not enough memory");
    i->type = PARAM_LINE;
    i->lineno = yylineno;
    i->name = n;
    i->param = p;
    i->value = 0;
    return (struct item *)i;
}

struct item *new_plf_item (char *n, struct param_list *p, char f)
{
    struct plf_item *i;

    i = malloc (sizeof (*i));
    if (i == NULL) yyerror ("Not enough memory");
    i->type = PARAM_F_LINE;
    i->lineno = yylineno;
    i->name = n;
    i->param = p;
    i->value = 0;
    i->flag = f;
    return (struct item *)i;
}

struct item *new_bl_item (char *n, struct param_list *p, struct item_list *s)
{
    struct bl_item *i;

    i = malloc (sizeof (*i));
    if (i == NULL) yyerror ("Not enough memory");
    i->type = BLOCK_LINE;
    i->lineno = yylineno;
    i->name = n;
    i->param = p;
    i->child = s;
    i->ind = 1;
    return (struct item *)i;
}

struct item_list *new_item_list (struct item *i, struct item_list *s)
{
    struct item_list *l;

    l = malloc (sizeof (*l));
    if (l == NULL) yyerror ("Not enough memory");
    l->i = i;
    l->next = s;
    return l;
}

struct param *new_num_param (char p, int v)
{
    struct num_param *i;

    i = malloc (sizeof (*i));
    if (i == NULL) yyerror ("Not enough memory");
    i->type = NUM_PARAM;
    i->par = p;
    i->value = v;
    return (struct param *)i;
}

struct param *new_ref_param (char p, char *r)
{
    struct ref_param *i;

    i = malloc (sizeof (*i));
    if (i == NULL) yyerror ("Not enough memory");
    i->type = REF_PARAM;
    i->par = p;
    i->refn = r;
    return (struct param *)i;
}

struct param_list *new_param_list (struct param_list *l, struct param *p)
{
    struct param_list *e;
    e = malloc (sizeof (*e));
    if (e == NULL) yyerror ("Not enough memory");
    e->p = p;
    e->next = l;
    return e;
}

void free_param_list (struct param_list *l)
{
    struct param_list *s;

    while (l) {
        if (l->p->type == REF_PARAM)
            free (((struct ref_param *)l->p)->refn);
        free (l->p);
        s = l->next;
        free (l);
        l = s;
    }
}

void sl_free (struct sl_item *i)
{
    free (i->name);
    free (i);
}

void pl_free (struct pl_item *i)
{
    free (i->name);
    free_param_list (i->param);
    free (i);
}

void plf_free (struct plf_item *i)
{
    if (i->name != NULL)
        free (i->name);
    free_param_list (i->param);
    free (i);
}

void bl_free (struct bl_item *i)
{
    free (i->name);
    if (i->param != NULL)
        free_param_list (i->param);
    free_item_list (i->child);
    free (i);
}

void print_ind (struct bl_item *i)
{
    char s[10];
    int sz;

    sz = (i->ind > 9) ? 9 : i->ind;
    memset (s, '\t', sz);
    s[sz] = 0;
    printf ("%s", s);
}

void print_num (char f, int sz)
{
    unsigned char s[8] = { 0 };
    int i;
    unsigned long long d;

    for (i = 0; i < sz; i++)
        s[i] = buf[i];

    d = *(unsigned long long *)s;
    if (f == 'd')
        printf ("%lld", d);
    else
        printf ("0x%llx", d);
}

void print_str (char f, int sz)
{
    char *str;

    if (sz == 0) { /* string is zero-terminated */
        (f == 'a') ? printf ("%s", buf) : printf ("%ls", buf);
    } else {
        str = malloc (sz + 2);
        memcpy (str, buf, sz);
        str[sz - 2] = 0;
        str[sz - 1] = 0;
        (f == 'a') ? printf ("%s", str) : printf ("%ls", str);
        free (str);
    }
}

void print_hex (int sz)
{

}

struct param *get_print_param (struct item *i)
{
    struct param *r;
    struct param_list *p = ((struct pl_item *)i)->param;
    while (p != NULL) {
        r = p->p;
        if (r->par == 'a' || r->par == 's' || r->par == 'x' ||
            r->par == 'd' || r->par == 'h')
            return r;
        p = p->next;
    }
    return NULL;
}

int get_param_size (char *name, struct item_list *l)
{
    return 0;
}

int print_param (struct item *i, struct bl_item *p, int silent)
{
    struct param *pr;
    int sz;
    void *str;

    if (buf_size == 0)
        return 0;

    pr = get_print_param (i);
    if (pr == NULL)
        yyerror ("Syntax error (%d) - unrecognized print format", i->lineno);

    sz = (pr->type == NUM_PARAM) ? ((struct num_param *)pr)->value :
      get_param_size (((struct ref_param *)pr)->refn, (p == NULL) ? root : p->child);

    if (sz > buf_size)
        sz = buf_size;

    if (silent)
        goto done;

    if (pr->par == 'd' || pr->par == 'x') {
        if (sz > 8)
            yyerror ("Syntax error (%d) - number print format bigger than 8 bytes", i->lineno);
        else
            print_num (pr->par, sz);
    } else if (pr->par == 'a' || pr->par == 's') {
        if (sz == 0) {
            if (pr->par == 'a')
                sz = (1 + strnlen ((char *)buf, buf_size)) * sizeof (char);
            else
                sz = (1 + wcsnlen ((wchar_t *)buf, buf_size >> 1)) * sizeof (wchar_t);

            if (sz > buf_size) {
                sz = buf_size;
                print_str (pr->par, sz);
            } else {
                print_str (pr->par, 0);
            }
        } else {
            print_str (pr->par, sz);
        }
    } else {
        if (sz == 0)
            sz = buf_size;
        print_hex (sz);
    }

done:
    buf_size -= sz;
    return sz;
}

void sl_print (struct sl_item *i, struct bl_item *p)
{
    printf ("\n");
    if (p != NULL)
        print_ind (p);
    printf ("%s", i->name);
}

void pl_print (struct pl_item *i, struct bl_item *p)
{
    printf ("\n");
    if (p != NULL)
        print_ind (p);
    printf ("%s: ", i->name);
    buf += print_param ((struct item *)i, p, 0);
}

void plf_print (struct plf_item *i, struct bl_item *p)
{
    char r[] = "Reserved";
    char *s;

    if (i->flag == '-') {
        buf += print_param ((struct item *)i, p, 1);
        return;
    }

    if (i->flag == '&') {
        printf ("\t");
    } else {
        printf ("\n");
        if (p != NULL)
            print_ind (p);
    }

    s = (i->flag == '$') ? r : i->name;
    printf ("%s: ", s);
    buf += print_param ((struct item *)i, p, 0);
}

void bl_print (struct bl_item *i, struct bl_item *p)
{
    if (p != NULL)
        i->ind = p->ind + 1;
    process_item_list (i->child, i);
}
