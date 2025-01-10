/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "esp_err.h"
#include "driver/rmt_encoder.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    rmt_encoder_handle_t encoder;
    rmt_channel_handle_t channel;
} dshot_handle_t;

esp_err_t rmt_dshot_init(dshot_handle_t *handle, gpio_num_t gpio_num);
esp_err_t rmt_dshot_write_throttle(dshot_handle_t handle, uint16_t throttle);

#ifdef __cplusplus
}
#endif
