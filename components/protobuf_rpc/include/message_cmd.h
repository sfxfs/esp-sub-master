#ifndef SUB_PB_RPC_CMD_H
#define SUB_PB_RPC_CMD_H

#include "navi_master.pb.h"

// for main
int message_cmd_init(void);

// for component itself
void message_thruster_cmd(ThrusterCommand *msg);
void message_pwmDev_cmd(PWMDevCommand *msg);

#endif //SUB_PB_RPC_CMD_H
