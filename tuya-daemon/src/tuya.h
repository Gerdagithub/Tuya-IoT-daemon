#ifndef TUYA_FOR_DAEMON_H
#define TUYA_FOR_DAEMON_H

#include <assert.h>
#include <tuya_error_code.h>
#include <system_interface.h>
#include <mqtt_client_interface.h>
#include <tuyalink_core.h>
#include <ubus_router.h>
#include <cJSON.h>
#include <argp_for_daemon.h>
#include <signal.h>
#include <stdbool.h>
#include <syslog.h>

extern volatile bool run_loop;

void signal_handler(int signal);
void tuya_init(struct Arguments arguments, tuya_mqtt_context_t *client, int *ret);
void send_data_about_memory(const char deviceId[], tuya_mqtt_context_t *client, struct MemData memory);
void tuya_loop_to_send_data(struct ubus_context *ctx, struct MemData memory, struct Arguments arguments, tuya_mqtt_context_t *client, 
                            uint32_t id, int *ret);

#endif