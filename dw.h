#ifndef __DUMPVIEW__H__
#define __DUMPVIEW__H__

struct item {
    int type;
    char *name;
    char format;
    int size;
    int lineno;
};

struct param {
    char type;
    int value;
    char *refname;
};

struct item_list {
    struct item *item;
    struct item_list *next;
};

sturct param_list {
    struct param *par;
    struct param_list *next;
};

struct block {
    int type;
    char *name;
    int count;
    struct param_list *params;
    struct item_list *items;
};



#endif
