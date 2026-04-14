/*******************************************************************************
 * @file    pdm_probe_logger.c
 * @brief   PC simulator mock -- PDM probe logger
 *
 *          Uses an LVGL timer to periodically read mock PDM frames, compute
 *          level metrics, and publish them to mic_presenter.  Replaces the
 *          FreeRTOS task used in real firmware.
 ******************************************************************************/
#include "pdm_probe_logger.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lvgl.h"
#include "mic_presenter.h"
#include "pdm_mic.h"

#define PDM_MOCK_TIMER_PERIOD_MS   100U   /* ~10 frames worth of data */
#define PDM_UI_FLOOR_ABS           80U
#define PDM_UI_CEIL_ABS            8000U

static lv_timer_t *s_timer = NULL;
static uint32_t s_frame_count = 0U;

typedef struct {
    uint32_t peak_abs;
    uint32_t avg_abs;
    uint32_t peak_tenth_pct_fs;
    uint32_t avg_tenth_pct_fs;
    uint32_t ui_pct;
} mic_level_t;

static uint32_t abs_i16(int16_t val)
{
    return (val < 0) ? (uint32_t)(-(int32_t)val) : (uint32_t)val;
}

static uint32_t to_tenth_pct_fs(uint32_t abs_value)
{
    return (abs_value * 1000U) / 32767U;
}

static uint32_t to_ui_pct(uint32_t avg_abs)
{
    if (avg_abs <= PDM_UI_FLOOR_ABS)  return 0U;
    if (avg_abs >= PDM_UI_CEIL_ABS)   return 100U;
    return ((avg_abs - PDM_UI_FLOOR_ABS) * 100U) / (PDM_UI_CEIL_ABS - PDM_UI_FLOOR_ABS);
}

static mic_level_t compute_level(const int16_t *samples, uint32_t sample_count)
{
    mic_level_t level = {0, 0, 0, 0, 0};
    uint64_t sum_abs = 0ULL;

    for (uint32_t i = 0U; i < sample_count; i++) {
        uint32_t a = abs_i16(samples[i]);
        if (a > level.peak_abs) {
            level.peak_abs = a;
        }
        sum_abs += (uint64_t)a;
    }

    level.avg_abs           = (sample_count > 0U)
                                  ? (uint32_t)(sum_abs / (uint64_t)sample_count)
                                  : 0U;
    level.peak_tenth_pct_fs = to_tenth_pct_fs(level.peak_abs);
    level.avg_tenth_pct_fs  = to_tenth_pct_fs(level.avg_abs);
    level.ui_pct            = to_ui_pct(level.avg_abs);

    return level;
}

static void pdm_probe_timer_cb(lv_timer_t *timer)
{
    (void)timer;

    pdm_mic_frame_t frame;
    memset(&frame, 0, sizeof(frame));

    cy_rslt_t rslt = pdm_mic_get_frame(&frame);
    if ((CY_RSLT_SUCCESS != rslt) || (NULL == frame.left) || (NULL == frame.right)) {
        return;
    }

    mic_level_t left  = compute_level(frame.left,  frame.sample_count);
    mic_level_t right = compute_level(frame.right, frame.sample_count);

    uint32_t sum_avg = left.avg_abs + right.avg_abs;
    int32_t balance_lr = 0;
    if (sum_avg > 0U) {
        balance_lr = ((int32_t)left.avg_abs - (int32_t)right.avg_abs)
                     * 100 / (int32_t)sum_avg;
    }

    s_frame_count++;

    mic_presenter_sample_t sample = {
        .frame_count            = s_frame_count,
        .left_peak_abs          = left.peak_abs,
        .left_avg_abs           = left.avg_abs,
        .right_peak_abs         = right.peak_abs,
        .right_avg_abs          = right.avg_abs,
        .left_peak_tenth_pct_fs = left.peak_tenth_pct_fs,
        .left_avg_tenth_pct_fs  = left.avg_tenth_pct_fs,
        .right_peak_tenth_pct_fs = right.peak_tenth_pct_fs,
        .right_avg_tenth_pct_fs = right.avg_tenth_pct_fs,
        .left_ui_pct            = left.ui_pct,
        .right_ui_pct           = right.ui_pct,
        .balance_lr             = balance_lr,
    };

    mic_presenter_publish_sample(&sample);
}

cy_rslt_t pdm_probe_logger_start(void)
{
    cy_rslt_t rslt = pdm_mic_init();
    if (CY_RSLT_SUCCESS != rslt) {
        return rslt;
    }

    if (s_timer != NULL) {
        /* Already running. */
        return CY_RSLT_SUCCESS;
    }

    s_timer = lv_timer_create(pdm_probe_timer_cb, PDM_MOCK_TIMER_PERIOD_MS, NULL);
    if (s_timer == NULL) {
        printf("[MOCK][MIC] TIMER_CREATE_FAIL\n");
        return CY_RSLT_TYPE_ERROR;
    }

    printf("[MOCK][MIC] LOGGER_START ok period_ms=%u\n",
           (unsigned)PDM_MOCK_TIMER_PERIOD_MS);
    return CY_RSLT_SUCCESS;
}
