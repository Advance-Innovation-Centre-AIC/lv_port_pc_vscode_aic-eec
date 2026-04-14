/*******************************************************************************
 * @file    mock_bmi270_reader.c
 * @brief   Mock BMI270 IMU — gentle sine-wave accelerometer + small gyro
 ******************************************************************************/
#include <math.h>
#include "bmi270_reader.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static uint32_t tick = 0;

cy_rslt_t bmi270_reader_init(mtb_hal_i2c_t *i2c_bus)
{
    (void)i2c_bus;
    tick = 0;
    return CY_RSLT_SUCCESS;
}

bool bmi270_reader_poll(bmi270_sample_t *out_sample)
{
    if (!out_sample) return false;

    double t = (double)tick * 0.02;   /* ~50 Hz virtual sample rate */

    out_sample->acc_g_x = 0.15f * sinf((float)(t * 1.0));
    out_sample->acc_g_y = 0.10f * sinf((float)(t * 0.7 + 1.0));
    out_sample->acc_g_z = 1.0f + 0.02f * sinf((float)(t * 0.3));

    out_sample->gyr_dps_x =  5.0f * sinf((float)(t * 0.5));
    out_sample->gyr_dps_y =  3.0f * sinf((float)(t * 0.8 + 0.5));
    out_sample->gyr_dps_z =  2.0f * cosf((float)(t * 0.4));

    out_sample->acc_mag_g = sqrtf(out_sample->acc_g_x * out_sample->acc_g_x +
                                  out_sample->acc_g_y * out_sample->acc_g_y +
                                  out_sample->acc_g_z * out_sample->acc_g_z);

    out_sample->gyr_mag_dps = sqrtf(out_sample->gyr_dps_x * out_sample->gyr_dps_x +
                                    out_sample->gyr_dps_y * out_sample->gyr_dps_y +
                                    out_sample->gyr_dps_z * out_sample->gyr_dps_z);

    out_sample->sample_count = tick;
    tick++;
    return true;
}

cy_rslt_t bmi270_reader_get_last_error(void)
{
    return CY_RSLT_SUCCESS;
}
