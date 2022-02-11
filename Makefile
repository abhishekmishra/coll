all: src test

src:
	$(MAKE) -C src

test:
	$(MAKE) -C test

clean:
	rm -f src/*.o
	rm -f src/*.so
	rm -f test/*.o
	rm -f test/test_all

run_tests:
	export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:./src; ./test/test_all

.PHONY: src test run_tests