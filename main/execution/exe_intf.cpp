#include "exe_intf.h"

#include <esp_log.h>

static const char *TAG = "ExecInterface";

using namespace SubMaster;

ExecInterface::ExecInterface(uint8_t channel, double max_val, double min_val)
    : m_channel(channel), m_max_val(max_val), m_min_val(min_val)
{
    set(0.0);
}

ExecInterface::~ExecInterface() { set(0.0); }

esp_err_t ExecInterface::set(double value)
{
    if (value > m_max_val)
    {
        ESP_LOGW(TAG, "Value %f exceeds max limit %f. Clamping to max.", value, m_max_val);
        value = m_max_val;
    }
    else if (value < m_min_val)
    {
        ESP_LOGW(TAG, "Value %f below min limit %f. Clamping to min.", value, m_min_val);
        value = m_min_val;
    }

    m_prv_val = m_cur_val;
    m_cur_val = value;

    return set_val(value);
}
