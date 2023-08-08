#ifndef ADDITIONAL_H
#define ADDITIONAL_H

#include "tuya.h"
#include "lua_data.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <string.h>
#include <syslog.h>
#include <cJSON.h>

void cjson_to_lua_table(lua_State *L, cJSON* item);
bool is_json(const char json_string[256]);

#endif