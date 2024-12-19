// stdlibs
#include <stdio.h>

// offical components
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// components
#include "dshot.h"
#include "aht30_app.h"
#include "pca9685_app.h"
#include "sub_intf_init.h"

// mains
#include "pb_rpc.h"
#include "navi_master.pb.h"

dshot_handle_t dshot_chan0;
dshot_handle_t dshot_chan1;
dshot_handle_t dshot_chan2;
dshot_handle_t dshot_chan3;
dshot_handle_t dshot_chan4;
dshot_handle_t dshot_chan5;
dshot_handle_t dshot_chan6;
dshot_handle_t dshot_chan7;

static int thruster_init(void)
{
    int ret = 0;
    ret += rmt_dshot_init(&dshot_chan0, 0);
    ret += rmt_dshot_init(&dshot_chan1, 1);
    ret += rmt_dshot_init(&dshot_chan2, 2);
    ret += rmt_dshot_init(&dshot_chan3, 3);
    ret += rmt_dshot_init(&dshot_chan4, 4);
    ret += rmt_dshot_init(&dshot_chan5, 5);
    ret += rmt_dshot_init(&dshot_chan6, 6);
    ret += rmt_dshot_init(&dshot_chan7, 7);
    return ret;
}

void app_main(void)
{

}
