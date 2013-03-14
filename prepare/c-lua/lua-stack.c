#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

// int main(int argc, char *argv[])
// {
//         char line[BUFSIZ];
//         lua_State *L = lua_open();
//         luaL_openlibs(L);
//         luaL_dofile(L, "./../zz.lua");
//         while (fgets(line, sizeof(line), stdin) != 0) printf("%s\n",line);

//         lua_close(L);
//         return 0;
// }

static void stackDump (lua_State *L)
{
	int i;
	int top = lua_gettop(L);
	for(i=1;i<=top;i++){
	/*repeatforeachlevel*/
		int t = lua_type(L, i);
		switch (t) {
			case LUA_TSTRING:
			 /* strings */
			printf("`%s'", lua_tostring(L, i));
			break;
			case LUA_TBOOLEAN:
			/* booleans */
			printf(lua_toboolean(L, i) ? "true" : "false");
			break;
			case LUA_TNUMBER:
			/* numbers */
			printf("%g", lua_tonumber(L, i));
			break;
			default:
			/* other values */
			printf("%s", lua_typename(L, t));
			break;
		}
		printf(" ");
		/* put a separator */
	}
	printf("\n");
	/* end the listing */
}

int main(int argc, char const *argv[])
{
	lua_State *L = lua_open();
	luaL_openlibs(L);
	char * a = "zzzz";
	char * b = "jjjj";
	luaL_loadfile(L, "./../zz.lua");
	lua_getglobal(L,"do_c");
	lua_pushstring(L, a);
	lua_pushstring(L, b);
	// lua_pcall(L, 2, 1, 0);
	// lua_cpcall(L,"do_c");
	// if (lua_pcall(L, 2, 1, 0) != 0)
	// 	printf("%s\n", 'F');
	stackDump(L);
	int j = lua_gettop(L);
	printf("%d\n", j);
	// int k = lua_checkstack(L, 100);
	// printf("%d\n", k);
	lua_settop(L,100);
	j = lua_gettop(L);
	printf("%d\n", j);
	lua_close(L);
	return 0;
}