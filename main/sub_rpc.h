#ifndef SUB_RPC_H
#define SUB_RPC_H

#include "esp_err.h"

#include "pb_common.h"

esp_err_t sub_rpc_init(void);
esp_err_t sub_rpc_start_thread(void);

#endif
