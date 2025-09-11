#include "ledc.h"

#include <esp_log.h>

static const char* TAG = "exe_ledc";

static esp_err_t ledc_init(ledc_channel_t channel, int io_num, ledc_timer_t timer)
{
    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel{};
    ledc_channel.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_channel.channel = channel; // 0 ~ 7
    ledc_channel.timer_sel = timer; // we need the same freq in each channel, so we use same timer
    ledc_channel.intr_type = LEDC_INTR_DISABLE;
    ledc_channel.gpio_num = io_num;
    ledc_channel.duty = 0; // give the mid signal to start the thrusters
    ledc_channel.hpoint = 0;

    return ledc_channel_config(&ledc_channel);
}

using namespace SubMaster;

LEDC::LEDC(uint8_t channel, int gpio_num, ledc_timer_t timer, ledc_timer_bit_t duty_res,
           uint32_t real_mid_val, uint32_t real_offset_val, double max_val, double min_val)
    : ExecInterface(channel, max_val, min_val), m_gpio_num(gpio_num), m_real_mid_val(real_mid_val),
      m_real_offset_val(real_offset_val), m_duty_res(duty_res)
{
    if (ledc_init(static_cast<ledc_channel_t>(channel), gpio_num, timer) != ESP_OK)
    {
        ESP_LOGE(TAG, "LEDC init failed for channel %d on GPIO %d", channel, gpio_num);
    }
}

esp_err_t LEDC::set_val(double value) { return ESP_OK; }
