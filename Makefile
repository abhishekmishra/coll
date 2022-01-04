all: src test

src:
	$(MAKE) -C src

test:
	$(MAKE) -C test

clean:
	$(MAKE) -C src clean
	$(MAKE) -C test clean

run_tests:
	export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:./src; ./test/test_all

.PHONY: src test run_tests