%option noyywrap nodefault yylineno batch
%{
#include <stdio.h>
%}

%%
[ \t\n]       /* ignore white space */
^!.*$      { printf ("line %d - as is string\n", yylineno); }
^#.*$      { printf ("line %d - comment string\n", yylineno); }

%%

