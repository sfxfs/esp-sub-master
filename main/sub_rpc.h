#ifndef SUB_RPC_H
#define SUB_RPC_H

#include "pb_common.h"

int sub_rpc_init(void);
int sub_rpc_start_thread(void);
int sub_rpc_send_resp(const pb_msgdesc_t *messagetype, void *message);

#endif
