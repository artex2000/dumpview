%option noyywrap nodefault yylineno batch
%{
#include <stdio.h>
#include "dumpview.h"
#include "dumpview.tab.h"
%}

%x FORM
%x SIZE
%x LINE

%%
[ \t]       /* ignore white space */
^#.*$       /* ignore comment line */
[a-zA-Z][a-zA-Z0-9]*    { printf ("line %d - got name %s\n", yylineno, yytext); }
":"                     { BEGIN FORM; }
"!"                     { BEGIN LINE; }
\n                      { return EOL; }
<<EOF>>                 { return END; }

<FORM>[ \t]
<FORM>"a"|"d"|"x"|"s"|"h" { yylval.f = yytext[0]; BEGIN SIZE; return FORMAT; } 
<FORM>.|\n    { printf ("line %d syntax error - unrecognized format\n", yylineno); yyterminate(); }

<SIZE>[ \t]
<SIZE>[0-9]+  { yylval.n = atoi(yytext); BEGIN INITIAL; return NUMBER; }
<SIZE>.       { printf ("line %d illegal format size\n", yylineno); yyterminate(); }

<LINE>.*$     { yylval.s = yytext; BEGIN INITIAL; return NAME; }

%%

void yyerror (char *s, ...)
{
    printf ("error - %s\n", s);
}


