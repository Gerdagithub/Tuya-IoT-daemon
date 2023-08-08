#include "lua_data.h"

extern volatile bool run_loop;
struct luaScripts LuaScripts;

// Method that loads all lua scripts from /usr/bin/scripts_for_lua/ directory
int load_lua_scripts()
{
    int ret = 0;

    char temp[300], directory[256];
    strcpy(directory, LUA_SCRIPTS_DIR);

    DIR *d;
    struct dirent *dir;
    d = opendir(directory);
    if(!d) {
        syslog(LOG_ERR, "Unable to open Lua scripts dir");
        ret = 1;
        return;
    }

    while ((dir = readdir(d)) != NULL) {
        if (strlen(dir->d_name) > 4 && strcmp(dir->d_name + strlen(dir->d_name) - 4, ".lua") == 0 && LuaScripts.amountOfScripts < MAX_SCRIPTS) {
            sprintf(temp, "%s/%s", directory, dir->d_name);

            LuaScripts.luaState[LuaScripts.amountOfScripts] = luaL_newstate();
            luaL_openlibs(LuaScripts.luaState[LuaScripts.amountOfScripts]);

            strncpy(LuaScripts.title[LuaScripts.amountOfScripts], dir->d_name,
            MAX_SCRIPT_NAME_LENGTH);

            if (luaL_loadfile(LuaScripts.luaState[LuaScripts.amountOfScripts], temp) ||
                lua_pcall(LuaScripts.luaState[LuaScripts.amountOfScripts], 0, 0, 0)) {

                syslog(LOG_USER | LOG_ERR, "Error loading Lua script %s: %s\n", dir->d_name,
                lua_tostring(LuaScripts.luaState[LuaScripts.amountOfScripts], -1));

                ret = 1;
                continue;
            }

            lua_getglobal(LuaScripts.luaState[LuaScripts.amountOfScripts], "get_data");
            if (!lua_isfunction(LuaScripts.luaState[LuaScripts.amountOfScripts], -1)){
                lua_close(LuaScripts.luaState[LuaScripts.amountOfScripts]);
                continue;
            }
            lua_call_method_without_args(LuaScripts.luaState[LuaScripts.amountOfScripts], "init");

            LuaScripts.amountOfScripts++;
        }
    }
    closedir(d);

    return ret;
}

int call_lua_method(lua_State *L, const char *fileName, const char *methodName, char *jsonToSendToTuya[256]) 
{
    char temp[300];
    sprintf(temp, "%s/%s", LUA_SCRIPTS_DIR, fileName);
    if (luaL_loadfile(L, temp) || lua_pcall(L, 0, 0, 0)) {
        syslog(LOG_USER | LOG_ERR, "Error loading Lua script %s: %s\n", fileName, lua_tostring(L, -1));
        return 1;
    }

    lua_getglobal(L, methodName);
    lua_pushvalue(L, -2);

    int err = lua_pcall(L, 1, 1, 0);
    if (err != 0) {
        syslog(LOG_USER | LOG_ERR, "Error calling Lua method: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
        return 1;
    }

    if (lua_isnil(L, -1) || !lua_isstring(L, -1)){
        syslog(LOG_USER | LOG_INFO, "Message from lua method was not in correct format");
        return 0;
    }

    strcpy(jsonToSendToTuya, luaL_checkstring(L, -1));
    lua_pop(L, 1);
    return 0;
}

int lua_call_method_without_args(lua_State *luaState, char methodName[MAX_FUNC_NAME_LENGTH])
{
    lua_getglobal(luaState, methodName);
    if (!lua_isfunction(luaState, -1))
        return 0;

    if (lua_pcall(luaState, 0, 0, 0)) {
        syslog(LOG_USER | LOG_ERR, "Error calling function from lua: %s",
        lua_tostring(luaState, -1));
           return 1;
    }

    return 0;
}

int lua_get_scripts_data(char deviceId[50], tuya_mqtt_context_t *client)
{
    int ret = 0;

    for (int i = 0; i < LuaScripts.amountOfScripts; i++){
        lua_getglobal(LuaScripts.luaState[i], "get_data");
        if (!lua_isfunction(LuaScripts.luaState[i], -1))
            continue;

        if (lua_pcall(LuaScripts.luaState[i], 0, 1, 0)) {
            syslog(LOG_USER | LOG_ERR, "Error calling function from lua: %s",
            lua_tostring(LuaScripts.luaState[i], -1));

                ret = 1;
        }

        if (!lua_isnil(LuaScripts.luaState[i], -1) && lua_isstring(LuaScripts.luaState[i], -1)
            && is_json(luaL_checkstring(LuaScripts.luaState[i], -1)))
                tuyalink_thing_property_report(client, deviceId, luaL_checkstring(LuaScripts.luaState[i], -1));
            else
                syslog(LOG_USER | LOG_INFO, "lua script returned not json");

        lua_pop(LuaScripts.luaState[i], 1);
    }

    return ret;
}

int lua_unload_scripts()
{
    int ret = 0;

    for (int i = 0; i < LuaScripts.amountOfScripts; i++){
        ret = lua_call_method_without_args(LuaScripts.luaState[i], "deinit");

        lua_close(LuaScripts.luaState[i]);
    }

    return ret;
}