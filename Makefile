.PHONY: trie

trie:
	$(MAKE) -C src
	cp src/main $@

baskervilles.txt:
	wget -O $@ https://www.gutenberg.org/ebooks/3070.txt.utf-8