#ifndef __DUMP_VIEW_H_
#define  __DUMP_VIEW_H_

#define SINGLE_LINE 0
#define PARAM_LINE  1
#define PARAM_F_LINE  2
#define BLOCK_LINE  3

#define NUM_PARAM 0
#define REF_PARAM 1

extern int yylineno;
extern FILE *yyin;

void yyerror (char *s, ...);
int yylex (void);

struct param {
    int type;      /*node type*/
    char par;
};

struct num_param {
    int type;      /*node type*/
    char par;
    int value;
};

struct ref_param {
    int type;      /*node type*/
    char par;
    char *refn;
};

struct param_list {
    struct param *p;        /* item */
    struct param_list *next;
};

struct item {
    int type;      /*node type*/
    int lineno;
};

struct item_list {
    struct item *i;        /* item */
    struct item_list *next;
};

struct sl_item { /* single-line data */
    int type;
    int lineno;
    char *name;
};

struct pl_item { /* param-line data */
    int type;
    int lineno;
    char *name;
    int value;
    struct param_list *param;
    int bytes;  /* number of bytes consumed */
};

struct plf_item { /* param-line data with flags */
    int type;
    int lineno;
    char *name;
    int value;
    struct param_list *param;
    int bytes;
    char flag;
};

struct bl_item { /* block data */
    int type;
    int lineno;
    char *name;
    int ind;    /* block indent */
    struct param_list *param;
    struct item_list *child;    /* child list */
};


struct item *new_sl_item (char *name);
struct item *new_pl_item (char *name, struct param_list *par);
struct item *new_plf_item (char *name, struct param_list *par, char flag);
struct item *new_bl_item (char *name, struct param_list *par, struct item_list *s);
struct item_list *new_item_list (struct item *i, struct item_list *l);
void process_item_list (struct item_list *l, struct bl_item *p);
void free_item_list (struct item_list *l);
void sl_free (struct sl_item *i);
void pl_free (struct pl_item *i);
void plf_free (struct plf_item *i);
void bl_free (struct bl_item *i);

struct param *new_num_param (char p, int v);
struct param *new_ref_param (char p, char *r);
struct param_list *new_param_list (struct param_list *l, struct param *p);
void free_param_list (struct param_list *l);
void free_param (struct param *p);

void sl_print (struct sl_item *i, struct bl_item *p);
void pl_print (struct pl_item *i, struct bl_item *p);
void plf_print (struct plf_item *i, struct bl_item *p);
void bl_print (struct bl_item *i, struct bl_item *p);
#endif

