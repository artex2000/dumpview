%option noyywrap nodefault yylineno batch
%{
#include <stdio.h>
#include "dumpview.h"
#include "dumpview.tab.h"
%}

%x PARAM
%x LINE

%%
[ \t\n]       /* ignore white space */
^#.*$       /* ignore comment line */
[a-zA-Z][a-zA-Z0-9]*    { yylval.s = strdup(yytext); return NAME; }
"{"                     { BEGIN PARAM; return yytext[0]; }
"["|"]"                 { return yytext[0]; }
"-"|"&"|"$"             { return yytext[0]; }
"!"                     { BEGIN LINE; return yytext[0]; }

<PARAM>[ \t\n]
<PARAM>[0-9]+                { yylval.n = atoi(yytext); return NUMBER; }
<PARAM>"a"|"d"|"x"|"s"|"h"   { yylval.f = yytext[0]; return FORMAT; } 
<PARAM>[a-zA-Z][a-zA-Z0-9]*  { yylval.s = strdup (yytext); return REF; }
<PARAM>"}"                   { BEGIN INITIAL; return yytext[0]; }
<PARAM>":"                   { return yytext[0]; }
<PARAM>.                     { printf ("line %d syntax error - unrecognized format\n", yylineno); yyterminate(); }

<LINE>.*$                    { yylval.s = strdup (yytext); BEGIN INITIAL; return NAME; }

%%

void yyerror (char *s, ...)
{
    printf ("error (line:%d) - %s\n", yylineno, s);
}


