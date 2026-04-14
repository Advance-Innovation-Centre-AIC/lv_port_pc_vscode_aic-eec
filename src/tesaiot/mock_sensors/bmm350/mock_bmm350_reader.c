/*******************************************************************************
 * @file    mock_bmm350_reader.c
 * @brief   Mock BMM350 magnetometer — slowly rotating heading, realistic XYZ
 ******************************************************************************/
#include <math.h>
#include "bmm350_reader.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static uint32_t tick = 0;

cy_rslt_t bmm350_reader_init(I3C_CORE_Type *i3c_hw, cy_stc_i3c_context_t *i3c_context)
{
    (void)i3c_hw;
    (void)i3c_context;
    tick = 0;
    return CY_RSLT_SUCCESS;
}

bool bmm350_reader_poll(bmm350_sample_t *out_sample)
{
    if (!out_sample) return false;

    double t = (double)tick * 0.02;

    /* Heading rotates slowly through 0-360 degrees */
    float heading = fmodf((float)(t * 6.0), 360.0f);   /* ~6 deg/s */
    if (heading < 0.0f) heading += 360.0f;

    float heading_rad = heading * (float)(M_PI / 180.0);

    /* Earth's field ~25-65 uT; simulate ~45 uT horizontal component */
    out_sample->x_ut = 45.0f * cosf(heading_rad) + 1.0f * sinf((float)(t * 0.3));
    out_sample->y_ut = 45.0f * sinf(heading_rad) + 0.8f * cosf((float)(t * 0.4));
    out_sample->z_ut = -20.0f + 0.5f * sinf((float)(t * 0.2));   /* vertical component */
    out_sample->temperature_c = 24.0f + 0.3f * sinf((float)(t * 0.05));

    out_sample->heading_deg = heading;
    out_sample->field_strength_ut = sqrtf(out_sample->x_ut * out_sample->x_ut +
                                          out_sample->y_ut * out_sample->y_ut +
                                          out_sample->z_ut * out_sample->z_ut);
    out_sample->sample_count = tick;

    tick++;
    return true;
}

cy_rslt_t bmm350_reader_get_last_error(void)
{
    return CY_RSLT_SUCCESS;
}

void bmm350_reader_start_calibration(void)
{
    /* No-op on PC simulator */
}

void bmm350_reader_get_calibration_status(bmm350_calibration_status_t *out_status)
{
    if (!out_status) return;
    out_status->active = false;
    out_status->done = true;
    out_status->sample_progress_pct = 100;
    out_status->coverage_progress_pct = 100;
    out_status->remaining_samples = 0;
}
