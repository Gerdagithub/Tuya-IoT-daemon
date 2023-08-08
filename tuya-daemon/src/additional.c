#include "additional.h"

extern struct luaScripts LuaScripts;
extern struct tuya TuyaData;

void cjson_to_lua_table(lua_State *L, cJSON *item) 
{
    cJSON* object_item;
    if (item == NULL)
        return;

    switch (item->type) {
        case cJSON_NULL:
            lua_pushnil(L);
            lua_settable(L, -3);
            break;

        case cJSON_False:
            lua_pushboolean(L, 0);
            lua_settable(L, -3);
            break;

        case cJSON_True:
            lua_pushboolean(L, 1);
            lua_settable(L, -3);
            break;

        case cJSON_Number:
            lua_pushinteger(L, item->valuedouble);
            lua_settable(L, -3);
            break;

        case cJSON_String:
            lua_pushstring(L, item->valuestring);
            lua_settable(L, -3);
            break;

        case cJSON_Object:
            object_item = item->child;
            while (object_item != NULL) {
                if (strcmp(object_item->string, "inputParams")){
                    lua_pushstring(L, object_item->string);
                }   
                cjson_to_lua_table(L, object_item);
               
                object_item = object_item->next; 
            }
            break;
    }
}

bool is_json(const char json_string[256]) 
{
    cJSON *json = cJSON_Parse(json_string);
    if (json == NULL) {
        cJSON_Delete(json);
        return false;
    }

    cJSON_Delete(json);
    return true; 
}