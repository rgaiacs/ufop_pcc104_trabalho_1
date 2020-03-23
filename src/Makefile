.PHONY: test

#CFLAGS=-Wall -Wpedantic -std=c90
DEBUG=-g
LOG=-D LOG=1

main: main.c trie.o
	${CC} ${CFLAGS} ${DEBUG} ${LOG} $^ -o $@

trie.o: trie.c
	${CC} ${CFLAGS} ${DEBUG} ${LOG} -c $^ -o $@

test: main
	for t in $$(find test -type f -regex '.*txt$$' | sort); do ./main $$t < $${t}.input; done