INC=-I../src
LIB=-L../src -lcoll -lcmocka

all: test_all

test_all: test_all.o test_arraylist.o test_coll_arraylist_map.o
	gcc test_all.o test_arraylist.o test_coll_arraylist_map.o ${LIB} -o test_all

test_all.o: test_all.c 
	gcc -c test_all.c ${INC} -o test_all.o

test_arraylist.o: test_arraylist.c test_arraylist.h
	gcc -c test_arraylist.c ${INC} -o test_arraylist.o

test_coll_arraylist_map.o: test_coll_arraylist_map.c test_coll_arraylist_map.h
	gcc -c test_coll_arraylist_map.c ${INC} -o test_coll_arraylist_map.o

clean:
	rm -f *.o
	rm -f test_all

.PHONY: clean