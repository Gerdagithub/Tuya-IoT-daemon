#include <ubus_router.h>

extern volatile bool run_loop;

void memory_cb(struct ubus_request *req, int type, struct blob_attr *msg, int *ret)
{
	struct MemData *memoryData = (struct MemData *)req->priv;
	struct blob_attr *tb[__INFO_MAX];
	struct blob_attr *memory[__MEMORY_MAX];

	blobmsg_parse(info_policy, __INFO_MAX, tb, blob_data(msg), blob_len(msg));

	if (!tb[MEMORY_DATA]) {
		syslog(LOG_USER | LOG_ERR, "No memory data received\n");
		*ret = -1;
		return;
	}

	blobmsg_parse(memory_policy, __MEMORY_MAX, memory, blobmsg_data(tb[MEMORY_DATA]),
		          blobmsg_data_len(tb[MEMORY_DATA]));

	memoryData->total = blobmsg_get_u64(memory[TOTAL_MEMORY]);
    memoryData->free = blobmsg_get_u64(memory[FREE_MEMORY]);
}

void connect_to_ubus(struct ubus_context **ctx, int *ret)
{
	*ctx = ubus_connect(NULL);

	if (!(*ctx)){
		*ret = -1;
		return;
	}

	*ret = 0;
}