all: src test

src:
	$(MAKE) -C src

test:
	$(MAKE) -C test

clean:
	$(MAKE) -C src clean
	$(MAKE) -C test clean

run_tests:
	./test/test_all

.PHONY: src test run_tests