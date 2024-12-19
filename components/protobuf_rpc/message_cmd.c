#include "dshot.h"

#include "sdkconfig.h"

#include "message_cmd.h"

static dshot_handle_t dshot_chan0;
static dshot_handle_t dshot_chan1;
static dshot_handle_t dshot_chan2;
static dshot_handle_t dshot_chan3;
static dshot_handle_t dshot_chan4;
static dshot_handle_t dshot_chan5;
static dshot_handle_t dshot_chan6;
static dshot_handle_t dshot_chan7;

static int thruster_init(void)
{
    int ret = 0;
    ret += rmt_dshot_init(&dshot_chan0, CONFIG_SUB_PROTOBUF_THRUSTER0_PIN);
    ret += rmt_dshot_init(&dshot_chan1, CONFIG_SUB_PROTOBUF_THRUSTER1_PIN);
    ret += rmt_dshot_init(&dshot_chan2, CONFIG_SUB_PROTOBUF_THRUSTER2_PIN);
    ret += rmt_dshot_init(&dshot_chan3, CONFIG_SUB_PROTOBUF_THRUSTER3_PIN);
    ret += rmt_dshot_init(&dshot_chan4, CONFIG_SUB_PROTOBUF_THRUSTER4_PIN);
    ret += rmt_dshot_init(&dshot_chan5, CONFIG_SUB_PROTOBUF_THRUSTER5_PIN);
    ret += rmt_dshot_init(&dshot_chan6, CONFIG_SUB_PROTOBUF_THRUSTER6_PIN);
    ret += rmt_dshot_init(&dshot_chan7, CONFIG_SUB_PROTOBUF_THRUSTER7_PIN);
    return ret;
}

int message_cmd_init(void)
{
    int ret = 0;
    ret += thruster_init();
    // other cmd init ...
    return ret;
}

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
