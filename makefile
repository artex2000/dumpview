dumpview: dumpview.l dumpview.c
	flex dumpview.l
	cc -o $@ dumpview.c dumpview.lex.c
