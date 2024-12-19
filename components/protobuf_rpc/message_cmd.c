#include "dshot.h"

#include "message_cmd.h"

extern dshot_handle_t dshot_chan0;
extern dshot_handle_t dshot_chan1;
extern dshot_handle_t dshot_chan2;
extern dshot_handle_t dshot_chan3;
extern dshot_handle_t dshot_chan4;
extern dshot_handle_t dshot_chan5;
extern dshot_handle_t dshot_chan6;
extern dshot_handle_t dshot_chan7;

void message_thruster_cmd(ThrusterCommand *msg)
{
    if (msg->has_throttle0)
        rmt_dshot_write_throttle(dshot_chan0, msg->throttle0);
    if (msg->has_throttle1)
        rmt_dshot_write_throttle(dshot_chan1, msg->throttle1);
    if (msg->has_throttle2)
        rmt_dshot_write_throttle(dshot_chan2, msg->throttle2);
    if (msg->has_throttle3)
        rmt_dshot_write_throttle(dshot_chan3, msg->throttle3);
    if (msg->has_throttle4)
        rmt_dshot_write_throttle(dshot_chan4, msg->throttle4);
    if (msg->has_throttle5)
        rmt_dshot_write_throttle(dshot_chan5, msg->throttle5);
    if (msg->has_throttle6)
        rmt_dshot_write_throttle(dshot_chan6, msg->throttle6);
    if (msg->has_throttle7)
        rmt_dshot_write_throttle(dshot_chan7, msg->throttle7);
}

void message_arm_cmd(ArmCommand *msg)
{

}
