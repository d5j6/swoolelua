#include "../SwooleClient.h"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

static const char* SwooleLib = "Swoole.lib";

/* return a connection and an error message */
static int l_new_swooleclient(lua_State* L)
{
    SwooleClient** conn = (SwooleClient**)lua_newuserdata(L, sizeof(SwooleClient*));
	luaL_getmetatable(L, SwooleLib);
	lua_setmetatable(L, -2);

	*conn = new SwooleClient();
	return 1;
}

static SwooleClient* get_client(lua_State* L)
{
    SwooleClient** conn = (SwooleClient**)luaL_checkudata(L, 1, SwooleLib);
    if(conn)
    	return *conn;
    else
    	return NULL;
}


static int l_swooleclient_connect(lua_State* L)
{
    SwooleClient* client = get_client(L);
    if (!client) {
        lua_pushstring(L, "argument #1 is not a swoole client.");
        return 1;
    }

    const char* host=luaL_checkstring(L,2);
    int port = luaL_checknumber(L,3);

    if (client->SwConnect((char*)host,port))
        lua_pushnil(L);
    else
        lua_pushstring(L,"connect error");
    return 1;
}


static int l_swooleclient_sendRecv(lua_State* L)
{
    SwooleClient* client = get_client(L);
    if (!client) {
        lua_pushstring(L, "argument #1 is not a swoole client.");
        return 1;
    }

    size_t len=0;
    char* data=(char*)luaL_checklstring(L,2,&len);
    string recv;
    if (client->SendData(data,len,recv)>=0)
        lua_pushlstring(L,recv.c_str(),recv.length());
    else
        lua_pushnil(L);
    return 1;
}


static int l_swooleclient_send(lua_State* L)
{
    SwooleClient* client = get_client(L);
    if (!client) {
        lua_pushstring(L, "argument #1 is not a swoole client.");
        return 1;
    }

    size_t len=0;
    char* data=(char*)luaL_checklstring(L,2,&len);
    int rslt = client->SendData(data,len);
    lua_pushnumber(L,rslt);
    return 1;
}

static int l_swooleclient_close(lua_State* L)
{
    SwooleClient* client = get_client(L);
    if (!client) {
        lua_pushstring(L, "argument #1 is not a swoole client.");
        return 1;
    }
    client->SwClose();
    return 0;
}


static int l_swooleclient_isConnect(lua_State* L)
{
    SwooleClient* client = get_client(L);
    if (!client) {
        lua_pushstring(L, "argument #1 is not a swoole client.");
        return 1;
    }
    lua_pushboolean(L,client->IsConnected());
    return 1;
}

static int l_swooleclient_gc(lua_State* L)
{
    SwooleClient** conn = (SwooleClient**)luaL_checkudata(L, 1, SwooleLib);
    if(conn)
    {
    	delete *conn;
    	*conn=NULL;
    }
}

static const struct luaL_Reg swooleclient_f[] = {
    {"new", l_new_swooleclient},
    {NULL, NULL},
};

static const struct luaL_Reg swooleclient_m[] = {
    {"connect", l_swooleclient_connect},
    {"sendRecv", l_swooleclient_sendRecv},
    {"send", l_swooleclient_send},
    {"close", l_swooleclient_close},
    {"isConnect", l_swooleclient_isConnect},
    {"__gc", l_swooleclient_gc},
    {NULL, NULL},
};

int luaopen_luaswoole(lua_State* l)
{
    /* meta table for mysql client */
    luaL_newmetatable(l, SwooleLib);
    lua_pushvalue(l, -1);
    lua_setfield(l, -2, "__index");
	luaL_register(l, NULL, swooleclient_m);

	lua_createtable(l, 0, sizeof(swooleclient_f) / sizeof(swooleclient_f[0]));
	luaL_register(l, NULL, swooleclient_f);
    return 1;
}





