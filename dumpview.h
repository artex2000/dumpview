#ifndef __DUMP_VIEW_H_
#define  __DUMP_VIEW_H_

extern int yylineno;
extern FILE *yyin;

void yyerror (char *s, ...);
void yylex (void);

#endif

