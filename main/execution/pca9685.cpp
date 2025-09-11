#include "pca9685.h"

#include <pca9685.h> // from esp-idf-lib

#include <esp_log.h>

#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

static const char* TAG = "exe_pca9685";

using namespace SubMaster;

PCA9685::PCA9685(PCA9685&& other) noexcept
    : ExecInterface(other.m_channel, other.m_max_val, other.m_min_val), m_i2c_dev(other.m_i2c_dev),
      m_freq(other.m_freq)
{
    m_cur_val = other.m_cur_val;
    m_prv_val = other.m_prv_val;

    other.m_i2c_dev = nullptr;
    other.m_freq = 0;
    other.m_cur_val = 0.0;
    other.m_prv_val = 0.0;
}

PCA9685& PCA9685::operator=(PCA9685&& other) noexcept
{
    if (this != &other)
    {
        m_channel = other.m_channel;
        m_max_val = other.m_max_val;
        m_min_val = other.m_min_val;
        m_i2c_dev = other.m_i2c_dev;
        m_freq = other.m_freq;
        m_cur_val = other.m_cur_val;
        m_prv_val = other.m_prv_val;

        other.m_i2c_dev = nullptr;
        other.m_freq = 0;
        other.m_cur_val = 0.0;
        other.m_prv_val = 0.0;
    }
    return *this;
}

esp_err_t PCA9685::set_val(double value)
{
    if (m_i2c_dev == nullptr)
    {
        ESP_LOGE(TAG, "I2C device not initialized");
        return ESP_ERR_INVALID_STATE;
    }
    // Map the value to the pulse width
    // Refer to the freq to calculate the value to set, value is 0..4096
    // 1500us is the center point, 2000us is max, 1,000us is min
    uint16_t pulse_width_ms{static_cast<uint16_t>(m_real_mid_ms + (m_real_offset_ms * value))};
    double duty_cycle{pulse_width_ms / (1000.0 /* ms */ / m_freq)};
    uint16_t val{static_cast<uint16_t>(PCA9685_MAX_PWM_VALUE * duty_cycle)};

    if (pca9685_set_pwm_value(m_i2c_dev, m_channel, constrain(val, 0, PCA9685_MAX_PWM_VALUE)) !=
        ESP_OK)
        ESP_LOGE(TAG, "Could not set PWM value to ch%d", m_channel);

    return ESP_OK;
}
