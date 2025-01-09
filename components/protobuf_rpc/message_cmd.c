#include "esp_log.h"

#include "dshot.h"
#include "pca9685_app.h"

#include "sdkconfig.h"

#include "message_cmd.h"

static const char *TAG = "protobuf_commu_rpc_cmd";

static dshot_handle_t dshot_chan0, dshot_chan1, dshot_chan2, dshot_chan3,
                        dshot_chan4, dshot_chan5, dshot_chan6, dshot_chan7;


#if CONFIG_SUB_PROTOBUF_THRUSTERS_ENABLE

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

static int thruster_write(int channel, float value)
{
    uint16_t value_int = (uint16_t)(value * 2000.f);
    switch (channel)
    {
    case 0:return rmt_dshot_write_throttle(dshot_chan0, value_int);
    case 1:return rmt_dshot_write_throttle(dshot_chan1, value_int);
    case 2:return rmt_dshot_write_throttle(dshot_chan2, value_int);
    case 3:return rmt_dshot_write_throttle(dshot_chan3, value_int);
    case 4:return rmt_dshot_write_throttle(dshot_chan4, value_int);
    case 5:return rmt_dshot_write_throttle(dshot_chan5, value_int);
    case 6:return rmt_dshot_write_throttle(dshot_chan6, value_int);
    case 7:return rmt_dshot_write_throttle(dshot_chan7, value_int);
    default:return -1;
    }
}

#endif


#if CONFIG_SUB_ENABLE_PCA9685

static int pwmDev_init(void)
{
    if (0 != pca9685_app_init(PCA9685_ADDRESS_A000000, 50)) // 50Hz
        return -1;
    // pca9685_app_write ...
    return 0;
}

static int pwmDev_write(int channel, uint32_t value)
{
    return pca9685_app_write(channel, value);
}

#endif


int message_cmd_init(void)
{
    int ret = 0;
#if CONFIG_SUB_PROTOBUF_THRUSTERS_ENABLE
    ret += thruster_init();
#endif
#if CONFIG_SUB_ENABLE_PCA9685
    ret += pwmDev_init();
#endif
    // other cmd init ...
    return ret;
}

void message_thruster_cmd(ThrusterCommand *msg)
{
#if !CONFIG_SUB_PROTOBUF_THRUSTERS_ENABLE
    return;
#endif
    if (msg->has_throttle0)
    {
        ESP_LOGI(TAG, "dshot_chan0: %f", msg->throttle0);
        thruster_write(0, msg->throttle0);
    }
    if (msg->has_throttle1)
    {
        ESP_LOGI(TAG, "dshot_chan1: %f", msg->throttle1);
        thruster_write(1, msg->throttle1);
    }
    if (msg->has_throttle2)
    {
        ESP_LOGI(TAG, "dshot_chan2: %f", msg->throttle2);
        thruster_write(2, msg->throttle2);
    }
    if (msg->has_throttle3)
    {
        ESP_LOGI(TAG, "dshot_chan3: %f", msg->throttle3);
        thruster_write(3, msg->throttle3);
    }
    if (msg->has_throttle4)
    {
        ESP_LOGI(TAG, "dshot_chan4: %f", msg->throttle4);
        thruster_write(4, msg->throttle4);
    }
    if (msg->has_throttle5)
    {
        ESP_LOGI(TAG, "dshot_chan5: %f", msg->throttle5);
        thruster_write(5, msg->throttle5);
    }
    if (msg->has_throttle6)
    {
        ESP_LOGI(TAG, "dshot_chan6: %f", msg->throttle6);
        thruster_write(6, msg->throttle6);
    }
    if (msg->has_throttle7)
    {
        ESP_LOGI(TAG, "dshot_chan7: %f", msg->throttle7);
        thruster_write(7, msg->throttle7);
    }
}

void message_pwmDev_cmd(PWMDevCommand *msg)
{
#if !CONFIG_SUB_ENABLE_PCA9685
    return;
#endif
    if (msg->has_duty0)
    {
        ESP_LOGI(TAG, "pwm_chan0: %ld", msg->duty0);
        pwmDev_write(PCA9685_CHANNEL_0, msg->duty0);
    }
    if (msg->has_duty1)
    {
        ESP_LOGI(TAG, "pwm_chan1: %ld", msg->duty1);
        pwmDev_write(PCA9685_CHANNEL_1, msg->duty1);
    }
    if (msg->has_duty2)
    {
        ESP_LOGI(TAG, "pwm_chan2: %ld", msg->duty2);
        pwmDev_write(PCA9685_CHANNEL_2, msg->duty2);
    }
    if (msg->has_duty3)
    {
        ESP_LOGI(TAG, "pwm_chan3: %ld", msg->duty3);
        pwmDev_write(PCA9685_CHANNEL_3, msg->duty3);
    }
    if (msg->has_duty4)
    {
        ESP_LOGI(TAG, "pwm_chan4: %ld", msg->duty4);
        pwmDev_write(PCA9685_CHANNEL_4, msg->duty4);
    }
    if (msg->has_duty5)
    {
        ESP_LOGI(TAG, "pwm_chan5: %ld", msg->duty5);
        pwmDev_write(PCA9685_CHANNEL_5, msg->duty5);
    }
    if (msg->has_duty6)
    {
        ESP_LOGI(TAG, "pwm_chan6: %ld", msg->duty6);
        pwmDev_write(PCA9685_CHANNEL_6, msg->duty6);
    }
    if (msg->has_duty7)
    {
        ESP_LOGI(TAG, "pwm_chan7: %ld", msg->duty7);
        pwmDev_write(PCA9685_CHANNEL_7, msg->duty7);
    }
    if (msg->has_duty8)
    {
        ESP_LOGI(TAG, "pwm_chan8: %ld", msg->duty8);
        pwmDev_write(PCA9685_CHANNEL_8, msg->duty8);
    }
    if (msg->has_duty9)
    {
        ESP_LOGI(TAG, "pwm_chan9: %ld", msg->duty9);
        pwmDev_write(PCA9685_CHANNEL_9, msg->duty9);
    }
    if (msg->has_duty10)
    {
        ESP_LOGI(TAG, "pwm_chan10: %ld", msg->duty10);
        pwmDev_write(PCA9685_CHANNEL_10, msg->duty10);
    }
    if (msg->has_duty11)
    {
        ESP_LOGI(TAG, "pwm_chan11: %ld", msg->duty11);   
        pwmDev_write(PCA9685_CHANNEL_11, msg->duty11);
    }
    if (msg->has_duty12)
    {
        ESP_LOGI(TAG, "pwm_chan12: %ld", msg->duty12);
        pwmDev_write(PCA9685_CHANNEL_12, msg->duty12);
    }
    if (msg->has_duty13)
    {
        ESP_LOGI(TAG, "pwm_chan13: %ld", msg->duty13);
        pwmDev_write(PCA9685_CHANNEL_13, msg->duty13);
    }
    if (msg->has_duty14)
    {
        ESP_LOGI(TAG, "pwm_chan14: %ld", msg->duty14);
        pwmDev_write(PCA9685_CHANNEL_14, msg->duty14);
    }
    if (msg->has_duty15)
    {
        ESP_LOGI(TAG, "pwm_chan15: %ld", msg->duty15);
        pwmDev_write(PCA9685_CHANNEL_15, msg->duty15);
    }
}
