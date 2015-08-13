%.c : %.l
%.c : %.y

dumpview: dumpview.y dumpview.l dumpview.c
	bison -d dumpview.y
	flex -o dumpview.lex.c dumpview.l
	cc -o $@ dumpview.c dumpview.lex.c dumpview.tab.c

