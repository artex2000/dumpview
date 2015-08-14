%{
#include <stdio.h>
#include <stdlib.h>
#include "dumpview.h"
#define YYDEBUG 1
%}

%union {
    struct item *i;
    struct item_list *l;
    char *s;
    char f;
    int n;
}

%token <n> NUMBER
%token <f> FORMAT
%token <s> NAME

%type <i> item vitem bitem nitem
%type <l> list

%%

itemlist : /* nothing */
    | itemlist list { process ($2); }
    ;

list : item     { $$ = newlist ($1, NULL); }
    | item list { $$ = newlist ($1, $2); }
    ;

item : vitem    { $$ = $1; }
    | bitem     { $$ = $1; }
    | nitem     { $$ = $1; }
;

bitem : '[' NAME list ']' { $$ = newbitem ($2, 1, $3); }
    | '[' NAME NUMBER list ']' { $$ = newbitem ($2, $3, $4); }
    ;

vitem : NAME FORMAT NUMBER { $$ = newvitem ($1, $2, $3); }
;

nitem : '!' NAME { $$ = newitem ($2); }
;

%%
