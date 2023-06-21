#include <daemon.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ubus_router.h>
#include <tuya.h>
#include <argp_for_daemon.h>
#include <stdbool.h>

int main(int argc, char **argv)
{
    tuya_mqtt_context_t client_instance;
    tuya_mqtt_context_t* client = &client_instance;        

    int ret = OPRT_OK;

    struct MemData memory = { 0 };
    struct Arguments arguments; 

    strcpy(arguments.productId, "");
    strcpy(arguments.deviceId, "");
    strcpy(arguments.deviceSecret, "");
    arguments.becomeDaemon = false;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    const char *LOGNAME = "Tuya IoT daemon";
    openlog(LOGNAME, LOG_PID, LOG_USER); 

    if (arguments.becomeDaemon)
        ret = become_daemon(0);

    if(ret){
        syslog(LOG_USER | LOG_ERR, "An error occurred while trying to make the program a daemon process");
        closelog();
        return EXIT_FAILURE;
    }
    syslog(LOG_USER | LOG_INFO, "The program started");

    struct ubus_context *ctx;
	uint32_t id;

    connect_to_ubus(&ctx, &ret);
    tuya_init(arguments, client, &ret);
    tuya_loop_to_send_data(ctx, memory, arguments, client, id, &ret);

    ubus_free(ctx);
    tuya_mqtt_disconnect(client);
    tuya_mqtt_deinit(client);

    syslog(LOG_USER | LOG_INFO, "Tuya IoT daemon has ended");
    closelog();
    return ret;
}