#include "esp_log.h"

#include "dshot.h"
#include "pca9685_app.h"

#include "sdkconfig.h"

#include "message_cmd.h"

static const char *TAG = "protobuf_commu_rpc_cmd";

static dshot_handle_t dshot_chan0, dshot_chan1, dshot_chan2, dshot_chan3,
                        dshot_chan4, dshot_chan5, dshot_chan6, dshot_chan7;

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

static int pwmDev_init(void)
{
    if (0 != pca9685_app_init(PCA9685_ADDRESS_A000000, 50)) // 50Hz
        return -1;
    // pca9685_app_write ...
    return 0;
}

int message_cmd_init(void)
{
    int ret = 0;
    ret += thruster_init();
    ret += pwmDev_init();
    // other cmd init ...
    return ret;
}

void message_thruster_cmd(ThrusterCommand *msg)
{
    if (msg->has_throttle0)
    {
        ESP_LOGD(TAG, "dshot_chan0: %d", msg->throttle0);
        rmt_dshot_write_throttle(dshot_chan0, msg->throttle0);
    }
    if (msg->has_throttle1)
    {
        ESP_LOGD(TAG, "dshot_chan1: %d", msg->throttle1);
        rmt_dshot_write_throttle(dshot_chan1, msg->throttle1);
    }
    if (msg->has_throttle2)
    {
        ESP_LOGD(TAG, "dshot_chan2: %d", msg->throttle2);
        rmt_dshot_write_throttle(dshot_chan2, msg->throttle2);
    }
    if (msg->has_throttle3)
    {
        ESP_LOGD(TAG, "dshot_chan3: %d", msg->throttle3);
        rmt_dshot_write_throttle(dshot_chan3, msg->throttle3);
    }
    if (msg->has_throttle4)
    {
        ESP_LOGD(TAG, "dshot_chan4: %d", msg->throttle4);
        rmt_dshot_write_throttle(dshot_chan4, msg->throttle4);
    }
    if (msg->has_throttle5)
    {
        ESP_LOGD(TAG, "dshot_chan5: %d", msg->throttle5);
        rmt_dshot_write_throttle(dshot_chan5, msg->throttle5);
    }
    if (msg->has_throttle6)
    {
        ESP_LOGD(TAG, "dshot_chan6: %d", msg->throttle6);
        rmt_dshot_write_throttle(dshot_chan6, msg->throttle6);
    }
    if (msg->has_throttle7)
    {
        ESP_LOGD(TAG, "dshot_chan7: %d", msg->throttle7);
        rmt_dshot_write_throttle(dshot_chan7, msg->throttle7);
    }
}

void message_pwmDev_cmd(PWMDevCommand *msg)
{
    if (msg->has_duty0)
    {
        ESP_LOGD(TAG, "pwm_chan0: %d", msg->duty0);
        pca9685_app_write(PCA9685_CHANNEL_0, msg->duty0);
    }
    if (msg->has_duty1)
    {
        ESP_LOGD(TAG, "pwm_chan1: %d", msg->duty1);
        pca9685_app_write(PCA9685_CHANNEL_1, msg->duty1);
    }
    if (msg->has_duty2)
    {
        ESP_LOGD(TAG, "pwm_chan2: %d", msg->duty2);
        pca9685_app_write(PCA9685_CHANNEL_2, msg->duty2);
    }
    if (msg->has_duty3)
    {
        ESP_LOGD(TAG, "pwm_chan3: %d", msg->duty3);
        pca9685_app_write(PCA9685_CHANNEL_3, msg->duty3);
    }
    if (msg->has_duty4)
    {
        ESP_LOGD(TAG, "pwm_chan4: %d", msg->duty4);
        pca9685_app_write(PCA9685_CHANNEL_4, msg->duty4);
    }
    if (msg->has_duty5)
    {
        ESP_LOGD(TAG, "pwm_chan5: %d", msg->duty5);
        pca9685_app_write(PCA9685_CHANNEL_5, msg->duty5);
    }
    if (msg->has_duty6)
    {
        ESP_LOGD(TAG, "pwm_chan6: %d", msg->duty6);
        pca9685_app_write(PCA9685_CHANNEL_6, msg->duty6);
    }
    if (msg->has_duty7)
    {
        ESP_LOGD(TAG, "pwm_chan7: %d", msg->duty7);
        pca9685_app_write(PCA9685_CHANNEL_7, msg->duty7);
    }
    if (msg->has_duty8)
    {
        ESP_LOGD(TAG, "pwm_chan8: %d", msg->duty8);
        pca9685_app_write(PCA9685_CHANNEL_8, msg->duty8);
    }
    if (msg->has_duty9)
    {
        ESP_LOGD(TAG, "pwm_chan9: %d", msg->duty9);
        pca9685_app_write(PCA9685_CHANNEL_9, msg->duty9);
    }
    if (msg->has_duty10)
    {
        ESP_LOGD(TAG, "pwm_chan10: %d", msg->duty10);
        pca9685_app_write(PCA9685_CHANNEL_10, msg->duty10);
    }
    if (msg->has_duty11)
    {
        ESP_LOGD(TAG, "pwm_chan11: %d", msg->duty11);   
        pca9685_app_write(PCA9685_CHANNEL_11, msg->duty11);
    }
    if (msg->has_duty12)
    {
        ESP_LOGD(TAG, "pwm_chan12: %d", msg->duty12);
        pca9685_app_write(PCA9685_CHANNEL_12, msg->duty12);
    }
    if (msg->has_duty13)
    {
        ESP_LOGD(TAG, "pwm_chan13: %d", msg->duty13);
        pca9685_app_write(PCA9685_CHANNEL_13, msg->duty13);
    }
    if (msg->has_duty14)
    {
        ESP_LOGD(TAG, "pwm_chan14: %d", msg->duty14);
        pca9685_app_write(PCA9685_CHANNEL_14, msg->duty14);
    }
    if (msg->has_duty15)
    {
        ESP_LOGD(TAG, "pwm_chan15: %d", msg->duty15);
        pca9685_app_write(PCA9685_CHANNEL_15, msg->duty15);
    }
}
