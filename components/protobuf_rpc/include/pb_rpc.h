#ifndef SUB_PB_RPC_H
#define SUB_PB_RPC_H

#include "pb_common.h"

int protobuf_commu_init(void);
int protobuf_commu_start_thread(void);
int protobuf_commu_send_resp(const pb_msgdesc_t *messagetype, void *message);

#endif
