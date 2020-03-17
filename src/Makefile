.PHONY: test

CFLAGS=-Wall -Wpedantic -std=c90 -g

main: main.c trie.o
	${CC} ${CFLAGS} $^ -o $@

trie.o: trie.c
	${CC} ${CFLAGS} -c $^ -o $@

test: main
	find test -type f -exec ./main {} \;