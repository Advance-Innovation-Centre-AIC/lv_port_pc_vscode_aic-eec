/*******************************************************************************
 * @file    mock_sht4x_reader.c
 * @brief   Mock SHT4x humidity + temperature sensor
 ******************************************************************************/
#include <math.h>
#include "sht4x_reader.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static uint32_t tick = 0;

cy_rslt_t sht4x_reader_init(mtb_hal_i2c_t *i2c_bus)
{
    (void)i2c_bus;
    tick = 0;
    return CY_RSLT_SUCCESS;
}

bool sht4x_reader_poll(sht4x_sample_t *out_sample)
{
    if (!out_sample) return false;

    double t = (double)tick * 0.02;

    out_sample->temperature_c = 26.0f + 1.0f * sinf((float)(t * 0.08));
    out_sample->humidity_rh   = 55.0f + 5.0f * sinf((float)(t * 0.06 + 0.5));
    out_sample->sample_count  = tick;

    tick++;
    return true;
}

cy_rslt_t sht4x_reader_get_last_error(void)
{
    return CY_RSLT_SUCCESS;
}
