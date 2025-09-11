#pragma once

#include "exe_intf.h"

#include <esp_err.h>

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include <utility>

namespace SubMaster
{
template <typename T> class Thruster
{
  public:
    Thruster(T&& exec) : m_exec(std::forward<T>(exec)) {}

    void set(double value)
    {
        ESP_LOGD("exe_thruster", "Setting thruster on channel %d to value %.2f",
                 m_exec.get_channel(), value);
        m_exec.set(value);
    }
    double get() const { return m_exec.get(); }

  private:
    T m_exec;
    // TODO: add more funcs
};
} // namespace SubMaster
