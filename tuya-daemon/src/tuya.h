#ifndef TUYA_FOR_DAEMON_H
#define TUYA_FOR_DAEMON_H

#include <assert.h>
#include <tuya_error_code.h>
#include <system_interface.h>
#include <mqtt_client_interface.h>
#include <tuyalink_core.h>
#include <cJSON.h>
#include <signal.h>
#include <stdbool.h>
#include <syslog.h>
#include <cJSON.h>
#include "lua_data.h"
#include "argp_for_daemon.h"

struct tuya{
    tuya_mqtt_context_t *client;
    char deviceId[50];
    char deviceSecret[50];
};

void signal_handler(int signal);
void tuya_init(struct tuya TuyaData, int *ret);
void on_messages(tuya_mqtt_context_t *context, void *user_data, const tuyalink_message_t *msg);
void tuya_loop(struct tuya TuyaData, int time);

#endif