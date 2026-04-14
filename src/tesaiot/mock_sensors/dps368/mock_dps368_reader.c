/*******************************************************************************
 * @file    mock_dps368_reader.c
 * @brief   Mock DPS368 barometric pressure + temperature sensor
 ******************************************************************************/
#include <math.h>
#include "dps368_reader.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static uint32_t tick = 0;

cy_rslt_t dps368_reader_init(mtb_hal_i2c_t *i2c_bus)
{
    (void)i2c_bus;
    tick = 0;
    return CY_RSLT_SUCCESS;
}

bool dps368_reader_poll(dps368_sample_t *out_sample)
{
    if (!out_sample) return false;

    double t = (double)tick * 0.02;

    out_sample->pressure_hpa    = 1013.25f + 0.5f * sinf((float)(t * 0.1));
    out_sample->temperature_c   = 25.0f + 1.5f * sinf((float)(t * 0.05));
    out_sample->sample_count    = tick;

    tick++;
    return true;
}

cy_rslt_t dps368_reader_get_last_error(void)
{
    return CY_RSLT_SUCCESS;
}
