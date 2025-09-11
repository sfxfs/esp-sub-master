#pragma once

#include "exe_intf.h"

#include <esp_err.h>
#include <driver/ledc.h>

namespace SubMaster
{
class LEDC : public ExecInterface
{
  public:
    LEDC(uint8_t channel, int gpio_num, ledc_timer_t timer, ledc_timer_bit_t duty_res,
         uint32_t real_mid_val, uint32_t real_offset_val, double max_val = 1.0,
         double min_val = -1.0);

  private:
    int m_gpio_num;
    uint32_t m_real_mid_val;
    uint32_t m_real_offset_val;
    ledc_timer_bit_t m_duty_res;

    esp_err_t set_val(double value) override;
};
} // namespace SubMaster
