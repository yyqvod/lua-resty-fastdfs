#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

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

void error (lua_State *L, const char *fmt, const char *str) {
	printf(fmt, str);
}

int main(int argc, char const *argv[])
{
	lua_State *L = lua_open();
	luaL_openlibs(L);
	if (luaL_loadfile(L, "./../zz.lua") || lua_pcall(L, 0, 0, 0))
		error(L, "cannot run configuration file: %s",
			lua_tostring(L, -1));
	int width,height;
	lua_getglobal(L, "do_lua_kk");
	lua_getglobal(L, "a");
	lua_getglobal(L, "do_lua_error");
	lua_getglobal(L, "b");
	lua_getglobal(L, "do_c_one");
	lua_getglobal(L, "do_c");
	lua_getglobal(L, "width");
	lua_getglobal(L, "height");
	// lua_getglobal(L, "do_c_two");
	stackDump(L);
	int k = 100;
	k = lua_pcall(L, 2, 1, 1);
	// k = lua_pcall(L, 0, 3, 0);
	printf("--->%d\n", k);
	stackDump(L);
	lua_close(L);
	return 0;
}