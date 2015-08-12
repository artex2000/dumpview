%option noyywrap nodefault yylineno
%{
#include <stdio.h>
%}

%%
[ \t]       /* ignore white space */
^!.*\n      { printf ("line %d - as is string\n", yylineno); }
^#.*\n      { printf ("line %d - comment string\n", yylineno); }
\n          { printf ("line %d - empty string\n", yylineno); }

%%

