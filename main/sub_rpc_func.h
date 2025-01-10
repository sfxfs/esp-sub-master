#ifndef SUB_RPC_FUNC_H
#define SUB_RPC_FUNC_H

#include "esp_err.h"

#include "navi_master.pb.h"

esp_err_t sub_rpc_handle_func_init(void);

void handle_message_thruster_cmd(ThrusterCommand *msg);
void handle_message_pwmDev_cmd(PWMDevCommand *msg);

#endif //SUB_RPC_FUNC_H
