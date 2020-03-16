.PHONY: trie

trie:
	$(MAKE) -C src
	cp src/main $@