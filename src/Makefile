all: libcoll.a

libcoll.a: arraylist.o coll_arraylist_map.o
	ar rcs libcoll.a arraylist.o coll_arraylist_map.o

arraylist.o: arraylist.c arraylist.h
	gcc -c arraylist.c -o arraylist.o

coll_arraylist_map.o: coll_arraylist_map.c coll_arraylist_map.h
	gcc -c coll_arraylist_map.c -o coll_arraylist_map.o

clean:
	rm -f *.o
	rm -f *.a

.PHONY: coll_arraylist_map arraylist clean
