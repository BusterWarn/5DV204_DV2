makeexpressions: makecleancomments makewordcount makerundfa

makecleancomments: cleancomments.c
	gcc -std=c99 -Wall -g -o cleancomments cleancomments.c

makewordcount: wordcount.c
	gcc -std=c99 -Wall -g -o wordcount wordcount.c

makerundfa: rundfa.c dfa.c
	gcc -std=c99 -Wall -g -o rundfa rundfa.c dfa.c
