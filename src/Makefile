LUA_ENABLED=1
APR_ENABLED=1
LIBS=			-shared $(if $(LUA_ENABLED), `pkg-config --libs lua5.3`) $(if $(APR_ENABLED),`pkg-config --libs apr-1`)
CFLAGS=			-Wall -fpic $(if $(LUA_ENABLED),`pkg-config --cflags lua5.3` -D LUA_ENABLED) $(if $(APR_ENABLED), `pkg-config --cflags apr-1` -D APR_ENABLED)

all: 			libcoll.so

libcoll.so: 		coll_arraylist.o coll_arraylist_map.o $(if $(LUA_ENABLED), coll_lualib_arraylist.o)
	gcc $(LIBS) -o libcoll.so coll_arraylist.o coll_arraylist_map.o $(if $(LUA_ENABLED), coll_lualib_arraylist.o)

coll_arraylist.o: 	coll_arraylist.c coll_arraylist.h
	gcc -c coll_arraylist.c $(CFLAGS) -o coll_arraylist.o

coll_arraylist_map.o: coll_arraylist_map.c coll_arraylist_map.h
	gcc -c coll_arraylist_map.c $(CFLAGS) -o coll_arraylist_map.o

ifdef LUA_ENABLED
coll_lualib_arraylist.o: coll_lualib_arraylist.c
	gcc -c coll_lualib_arraylist.c $(CFLAGS) -o coll_lualib_arraylist.o
endif

clean:
	rm -f *.o
	rm -f *.so

.PHONY: coll_arraylist_map arraylist clean
