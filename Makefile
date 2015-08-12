dumpview: dumpview.l dumpview.c
	flex -o dumpview.lex.c dumpview.l
	cc -o $@ dumpview.c dumpview.lex.c
