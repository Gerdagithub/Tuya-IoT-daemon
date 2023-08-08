#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "daemon.h"
#include "tuya.h"
#include "argp_for_daemon.h"
#include "lua_data.h"

extern struct tuya TuyaData;

int main(int argc, char **argv)
{
    tuya_mqtt_context_t client_instance;
    TuyaData.client = &client_instance;

    int ret = OPRT_OK;
    struct Arguments arguments; 

    strcpy(arguments.productId, "");
    strcpy(arguments.deviceId, "");
    strcpy(arguments.deviceSecret, "");
    arguments.becomeDaemon = false;

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    strcpy(TuyaData.deviceId, arguments.deviceId);
    strcpy(TuyaData.deviceSecret, arguments.deviceSecret);

    const char *LOGNAME = "Tuya IoT daemon";
    openlog(LOGNAME, LOG_PID, LOG_USER);

    if (arguments.becomeDaemon)
        ret = become_daemon(0);

    if(ret){
        syslog(LOG_USER | LOG_ERR, "An error occurred while trying to make the program a daemon process");
        goto cleanUp;
    }
    syslog(LOG_USER | LOG_INFO, "The program started");

    tuya_init(TuyaData, &ret);
    if (ret)
        goto cleanUp;
 
    ret = load_lua_scripts();

    tuya_loop(TuyaData, 5);

cleanUp:
    tuya_mqtt_disconnect(TuyaData.client);
    tuya_mqtt_deinit(TuyaData.client);
    ret = lua_unload_scripts();        

    syslog(LOG_USER | LOG_INFO, "Tuya IoT daemon has ended");
    closelog();

    return ret;
}