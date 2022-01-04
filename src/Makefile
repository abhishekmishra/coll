LIBS=			-shared $(if $(LUA_ENABLED), `pkg-config --libs lua5.3`)
CFLAGS=			-Wall -fpic $(if $(LUA_ENABLED),`pkg-config --cflags lua5.3` -D LUA_ENABLED)

all: 			libcoll.so

libcoll.so: 		arraylist.o coll_arraylist_map.o $(if $(LUA_ENABLED), lualib_arraylist.o)
	gcc $(LIBS) -o libcoll.so arraylist.o coll_arraylist_map.o $(if $(LUA_ENABLED), lualib_arraylist.o)

arraylist.o: 	arraylist.c arraylist.h
	gcc -c arraylist.c $(CFLAGS) -o arraylist.o

coll_arraylist_map.o: coll_arraylist_map.c coll_arraylist_map.h
	gcc -c coll_arraylist_map.c $(CFLAGS) -o coll_arraylist_map.o

ifdef LUA_ENABLED
lualib_arraylist.o: lualib_arraylist.c
	gcc -c lualib_arraylist.c $(CFLAGS) -o lualib_arraylist.o
endif

clean:
	rm -f *.o
	rm -f *.so

.PHONY: coll_arraylist_map arraylist clean
