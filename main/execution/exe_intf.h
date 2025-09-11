#pragma once

#include <esp_err.h>

namespace SubMaster
{
class ExecInterface
{
  public:
    ExecInterface(uint8_t channel, double max_val, double min_val);
    ~ExecInterface();

    esp_err_t set(double value);
    double get() const { return m_cur_val; }
    double get_prev() const { return m_prv_val; }
    uint8_t get_channel() const { return m_channel; }

  protected:
    uint8_t m_channel{0};
    double m_cur_val{0.0};
    double m_prv_val{0.0};
    double m_max_val{0.0};
    double m_min_val{0.0};

    virtual esp_err_t set_val(double value) = 0;
};
} // namespace SubMaster
