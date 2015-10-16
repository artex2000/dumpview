%{
#include <stdio.h>
#include <stdlib.h>
#include "dumpview.h"
#define YYDEBUG 1
%}

%union {
    struct item *i;
    struct item_list *il;
    struct param *p;
    struct param_list *pl;
    char *s;
    char f;
    int n;
}

%token <n> NUMBER
%token <f> FORMAT
%token <s> NAME
%token <s> REF

%type <i> item sl_item pl_item plf_item bl_item
%type <il> itemlist
%type <p> param num_param ref_param
%type <pl> paramlist

%%

mainlist : /* nothing */
    | mainlist itemlist { process_item_list ($2, NULL); }
    ;

itemlist : item     { $$ = new_item_list ($1, NULL); }
    | item itemlist { $$ = new_item_list ($1, $2); }
    ;

item : sl_item    { $$ = $1; }
    | pl_item     { $$ = $1; }
    | plf_item    { $$ = $1; }
    | bl_item     { $$ = $1; }
;

paramlist : '{' param '}'    { $$ = new_param_list (NULL, $2); }
    | '{' paramlist ',' param '}' { $$ = new_param_list ($2, $4); }
;

param : num_param { $$ = $1; }
    | ref_param   { $$ = $1; }
;

num_param : FORMAT ':' NUMBER { $$ = new_num_param ($1, $3); }
;

ref_param : FORMAT ':' REF { $$ = new_ref_param ($1, $3); }
;

bl_item : '[' NAME itemlist ']' { $$ = new_bl_item ($2, NULL, $3); }
    | '[' NAME paramlist itemlist ']' { $$ = new_bl_item ($2, $3, $4); }
    ;

pl_item : NAME paramlist { $$ = new_pl_item ($1, $2); }
;

sl_item : '!' NAME { $$ = new_sl_item ($2); }
;

plf_item : '&' NAME paramlist { $$ = new_plf_item ($2, $3, '&'); }
    | '-' NAME paramlist { $$ = new_plf_item ($2, $3, '-'); }
    | '$' paramlist { $$ = new_plf_item (NULL, $2, '$'); }
;

%%
