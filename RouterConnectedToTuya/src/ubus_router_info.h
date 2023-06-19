#ifndef UBUS_ROUTER_INFO_H
#define UBUS_ROUTER_INFO_H

#include <libubox/blobmsg_json.h>
#include <libubus.h>
#include <syslog.h>

struct MemData {
	int total;
    int free;
};

enum {
	TOTAL_MEMORY,
    FREE_MEMORY,
	__MEMORY_MAX,
};

enum {
	MEMORY_DATA,
	__INFO_MAX,
};

static const struct blobmsg_policy memory_policy[__MEMORY_MAX] = {
	[TOTAL_MEMORY]	  = { .name = "total", .type = BLOBMSG_TYPE_INT64 },
    [FREE_MEMORY]	  = { .name = "free", .type = BLOBMSG_TYPE_INT64 },
};

static const struct blobmsg_policy info_policy[__INFO_MAX] = {
	[MEMORY_DATA] = { .name = "memory", .type = BLOBMSG_TYPE_TABLE },
};

void board_cb(struct ubus_request *req, int type, struct blob_attr *msg, int *ret);

#endif