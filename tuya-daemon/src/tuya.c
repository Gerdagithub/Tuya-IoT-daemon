#include "tuya.h"
#include "lua_data.h"
#include <tuya_cacert.h>

struct tuya TuyaData;

volatile bool run_loop = true;
extern struct luaScripts LuaScripts;

void signal_handler(int signal) 
{
    run_loop = false;
}

void tuya_init(struct tuya TuyaData, int *ret)
{
    *ret = tuya_mqtt_init(TuyaData.client, &(const tuya_mqtt_config_t) {
        .host = "m1.tuyacn.com",
        .port = 8883,
        .cacert = tuya_cacert_pem,
        .cacert_len = sizeof(tuya_cacert_pem),
        .device_id = TuyaData.deviceId,
        .device_secret = TuyaData.deviceSecret,
        .keepalive = 100,
        .timeout_ms = 2000,
        .on_messages = on_messages,
    });

    if (*ret){
        syslog(LOG_USER | LOG_ERR, "tuya_mqtt_init failed");
        run_loop = false;
        return;
    }
    
    *ret = tuya_mqtt_connect(TuyaData.client);
    if (*ret){
        syslog(LOG_USER | LOG_ERR, "tuya_mqtt_connect failed");
        run_loop = false;
        return;
    }

    while(run_loop && !tuya_mqtt_connected(TuyaData.client)){
        tuya_mqtt_loop(TuyaData.client);
    }
}

void on_messages(tuya_mqtt_context_t *context, void *user_data, const tuyalink_message_t *msg)
{
    char json_string[300];

    switch (msg->type) {
    case THING_TYPE_ACTION_EXECUTE:
        
        strcpy(json_string, msg->data_string);
        cJSON *root = cJSON_Parse(json_string);
        if (root == NULL)
            return;

        cJSON* actionCode = cJSON_GetObjectItem(root, "actionCode");
        if (actionCode->type != cJSON_String) {
            syslog(LOG_USER | LOG_ERR, "Invalid JSON format");

            cJSON_Delete(root);
            return;
        }
        char token[2];
        strcpy(token, strtok(actionCode->valuestring, "_"));

        char fileName[100]; 

        strcpy(fileName, token);
        strcat(fileName, ".lua");

        strcpy(token, strtok(NULL, "_"));
        char methodName[40];
        strcpy(methodName, token);
    
        strcpy(json_string, msg->data_string);

        lua_State *LuaState = luaL_newstate();
        luaL_openlibs(LuaState);

        lua_newtable(LuaState);
        cjson_to_lua_table(LuaState, root, -1);

        char jsonToSendToTuya[256];

        // loading file that is needed for action in tuya
        char temp[300];
        sprintf(temp, "%s/%s", LUA_SCRIPTS_DIR, fileName);

        if (luaL_loadfile(LuaState, temp) || lua_pcall(LuaState, 0, 0, 0)) { 
            syslog(LOG_USER | LOG_ERR, "Error loading Lua script %s: %s\n", fileName,
            lua_tostring(LuaState, -1));
            return;
        }

        call_lua_method(LuaState, fileName, methodName, &jsonToSendToTuya); 
        tuyalink_thing_property_report(TuyaData.client, TuyaData.deviceId, jsonToSendToTuya);

        lua_close(LuaState);

    default:
        break;
    }
}

void tuya_loop(struct tuya TuyaData, int time)
{
    while (run_loop) {
        tuya_mqtt_loop(TuyaData.client);

        lua_get_scripts_data(TuyaData.deviceId, TuyaData.client);

        sleep(time);
    }
}

