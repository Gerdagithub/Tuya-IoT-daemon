#include <tuya.h>
#include <tuya_cacert.h>

volatile bool run_loop = true;

void signalHandler(int signal) 
{
    syslog(LOG_USER | LOG_INFO, "Tuya IoT daemon has ended");
    run_loop = false;
}

void tuya_init(struct Arguments arguments, tuya_mqtt_context_t *client, int *ret)
{
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    *ret = tuya_mqtt_init(client, &(const tuya_mqtt_config_t) {
        .host = "m1.tuyacn.com",
        .port = 8883,
        .cacert = tuya_cacert_pem,
        .cacert_len = sizeof(tuya_cacert_pem),
        .device_id = arguments.deviceId,
        .device_secret = arguments.deviceSecret,
        .keepalive = 100,
        .timeout_ms = 2000,
    });
    assert(*ret == OPRT_OK);

    *ret = tuya_mqtt_connect(client);
    assert(*ret == OPRT_OK);
}

void send_data_about_memory(const char deviceId[], tuya_mqtt_context_t *client, struct MemData memory)
{
    char jsonStr[60];
    int BfreeMemory = memory.total - memory.free;
    double MBfreeMemory = BfreeMemory * 1.0 / 1000000;
    snprintf(jsonStr, sizeof(jsonStr), "{\"Amount_of_RAM_in_use_B\":%d}", BfreeMemory);
    tuyalink_thing_property_report(client, deviceId, jsonStr);

    snprintf(jsonStr, sizeof(jsonStr), "{\"Amount_of_RAM_in_use_MB\":%.2f}", MBfreeMemory);
    tuyalink_thing_property_report(client, deviceId, jsonStr);
}

void tuya_loop_to_send_data(struct ubus_context *ctx, struct MemData memory, struct Arguments arguments, 
                            tuya_mqtt_context_t *client, uint32_t id, int *ret)
{
    while (run_loop) {
        /* Loop to receive packets and handle client keepalive */
        tuya_mqtt_loop(client);

        if (ubus_lookup_id(ctx, "system", &id)) 
        {
            *ret = -1;
            syslog(LOG_USER | LOG_ERR, "ubus_lookup_id failed\n");
        } else if (ubus_invoke(ctx, id, "info", NULL, board_cb, &memory, 3000)) {
                syslog(LOG_USER | LOG_ERR, "cannot request memory info from procd\n");
                *ret = -1;
        } else {

            send_data_about_memory(arguments.deviceId, client, memory);
        }
    }
}
