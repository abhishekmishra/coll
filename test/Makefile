LUA_ENABLED=1
APR_ENABLED=1
CFLAGS=-Wall -fpic -I../src `pkg-config --cflags cmocka` $(if $(LUA_ENABLED),`pkg-config --cflags lua5.3` -D LUA_ENABLED) $(if $(APR_ENABLED),`pkg-config --cflags apr-1` -D APR_ENABLED)
LIB=-L../src -lcoll `pkg-config --libs cmocka` $(if $(LUA_ENABLED), `pkg-config --libs lua5.3`) $(if $(APR_ENABLED), `pkg-config --libs apr-1`)

all: test_all

test_all: test_all.o test_coll_arraylist.o test_coll_arraylist_map.o  $(if $(LUA_ENABLED), test_coll_lualib_arraylist.o)
	gcc test_all.o test_coll_arraylist.o test_coll_arraylist_map.o ${LIB} $(if $(LUA_ENABLED), test_coll_lualib_arraylist.o) -o test_all

test_all.o: test_all.c 
	gcc -c test_all.c ${CFLAGS} -o test_all.o

test_coll_arraylist.o: test_coll_arraylist.c test_coll_arraylist.h
	gcc -c test_coll_arraylist.c ${CFLAGS} -o test_coll_arraylist.o

test_coll_arraylist_map.o: test_coll_arraylist_map.c test_coll_arraylist_map.h
	gcc -c test_coll_arraylist_map.c ${CFLAGS} -o test_coll_arraylist_map.o

ifdef LUA_ENABLED
test_coll_lualib_arraylist.o: test_coll_lualib_arraylist.c test_coll_lualib_arraylist.h
	gcc -c test_coll_lualib_arraylist.c $(CFLAGS) -o test_coll_lualib_arraylist.o
endif

clean:
	rm -f *.o
	rm -f test_all

.PHONY: clean