#ifndef lua_H
#define lua_H

#include "tuya.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdbool.h>
#include <syslog.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#define MAX_SCRIPTS 20
#define MAX_SCRIPT_NAME_LENGTH 30
#define MAX_FUNCS 40
#define MAX_FUNC_NAME_LENGTH 30
#define LUA_SCRIPTS_DIR "/usr/bin/tuya-daemon/scripts_for_lua"
// #define LUA_SCRIPTS_DIR "/home/studentas/Documents/rutx_src/package/tuya-daemon2/lua"

struct luaScripts{
    int amountOfScripts;
    lua_State *luaState[MAX_SCRIPTS];
    char title[MAX_SCRIPT_NAME_LENGTH][MAX_SCRIPTS];
};

int load_lua_scripts();
int call_lua_method(lua_State *L, const char *fileName, const char *methodName, char *jsonToSendToTuya[256]);
int lua_get_scripts_data(char deviceId[50], tuya_mqtt_context_t *client);
int lua_call_method_without_args(lua_State *luaState, char methodName[MAX_FUNC_NAME_LENGTH]);
int lua_unload_scripts();

#endif