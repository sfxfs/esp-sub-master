#pragma once

#include "exe_intf.h"

#include <i2cdev.h>

#include <esp_err.h>

namespace SubMaster
{
class PCA9685 : public ExecInterface
{
  public:
    PCA9685(i2c_dev_t* dev, uint8_t channel, uint16_t freq, float real_mid_ms = 1.5,
            float real_offset_ms = 0.5, double max_val = 1.0, double min_val = -1.0)
        : ExecInterface(channel, max_val, min_val), m_i2c_dev(dev), m_freq(freq),
          m_real_mid_ms(real_mid_ms), m_real_offset_ms(real_offset_ms) {};
    PCA9685(PCA9685&& other) noexcept;
    PCA9685& operator=(PCA9685&& other) noexcept;

  private:
    i2c_dev_t* m_i2c_dev{nullptr};
    uint16_t m_freq{};
    float m_real_mid_ms{};
    float m_real_offset_ms{};

    esp_err_t set_val(double value) override;
};
} // namespace SubMaster
