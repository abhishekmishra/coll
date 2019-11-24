#include "arraylist.h"

#ifdef LUA_ENABLED

#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

void set_lua_convertor(arraylist* l, arraylist_item_to_lua_object* convert_to_lua) {
	if (l != NULL) {
		l->convert_to_lua = convert_to_lua;
	}
}

extern void convert_to_lua_array(arraylist* list, lua_State* L) {
	if (list->convert_to_lua == NULL) {
		return;
	}
	else {
		size_t len = arraylist_length(list);
		//printf("convert array of length %d\n", len);
		lua_createtable(L, len, 0);
		for (size_t i = 0; i < len; i++) {
			//printf("convert value @ %uL\n", i);
			list->convert_to_lua(L, i, arraylist_get(list, i));
		}
	}
}

#endif //LUA_ENABLED
