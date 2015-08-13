%.c : %.l

dumpview: dumpview.lex.c dumpview.c
	cc -o $@ dumpview.c dumpview.lex.c
dumpview.lex.c: dumpview.l
	flex -o dumpview.lex.c dumpview.l

