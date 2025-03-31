#pragma once

#include "esp_err.h"

typedef struct wt_jy901_data
{
    float Acc[3];
    float Gyro[3];
    float Angle[3];
} wt_jy901_data_t;

esp_err_t jy901_init(void);

esp_err_t jy901_routine_start(void);
