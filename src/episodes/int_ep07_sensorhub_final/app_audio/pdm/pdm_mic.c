/*******************************************************************************
 * @file    pdm_mic.c
 * @brief   PC simulator mock -- PDM microphone
 *
 *          Generates simulated stereo audio using sine waves with added noise.
 *          Returns frames of PDM_MIC_FRAME_SAMPLES_PER_CHANNEL samples per
 *          channel, matching the real firmware API.
 ******************************************************************************/
#include "pdm_mic.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static bool s_initialized = false;
static uint32_t s_frame_index = 0U;

static int16_t s_left_buf[PDM_MIC_FRAME_SAMPLES_PER_CHANNEL];
static int16_t s_right_buf[PDM_MIC_FRAME_SAMPLES_PER_CHANNEL];

/* Simple xorshift32 PRNG for deterministic noise */
static uint32_t s_rng_state = 0xDEADBEEFU;

static int16_t mock_noise(void)
{
    s_rng_state ^= s_rng_state << 13U;
    s_rng_state ^= s_rng_state >> 17U;
    s_rng_state ^= s_rng_state << 5U;
    return (int16_t)((s_rng_state & 0xFFFFU) - 0x8000U) >> 6;
}

cy_rslt_t pdm_mic_init(void)
{
    s_frame_index = 0U;
    s_rng_state   = 0xDEADBEEFU;
    s_initialized = true;

    printf("[MOCK][MIC] INIT_OK fs=%u frame_samples=%u ch=2(stereo)\n",
           (unsigned)PDM_MIC_SAMPLE_RATE_HZ,
           (unsigned)PDM_MIC_FRAME_SAMPLES_PER_CHANNEL);

    return CY_RSLT_SUCCESS;
}

cy_rslt_t pdm_mic_get_frame(pdm_mic_frame_t *frame)
{
    if ((frame == NULL) || !s_initialized) {
        return CY_RSLT_TYPE_ERROR;
    }

    /* Base time in seconds for this frame. */
    double t_base = (double)s_frame_index
                    * (double)PDM_MIC_FRAME_SAMPLES_PER_CHANNEL
                    / (double)PDM_MIC_SAMPLE_RATE_HZ;

    /* Left channel: 440 Hz tone + slow amplitude modulation + noise.
     * Right channel: 330 Hz tone + different modulation + noise.
     * This produces visually interesting and asymmetric levels.
     */
    for (uint32_t i = 0U; i < PDM_MIC_FRAME_SAMPLES_PER_CHANNEL; i++) {
        double t = t_base + (double)i / (double)PDM_MIC_SAMPLE_RATE_HZ;

        /* Amplitude envelope: 0.15 .. 0.65 of full scale */
        double env_l = 0.40 + 0.25 * sin(2.0 * M_PI * 0.5 * t);
        double env_r = 0.40 + 0.25 * sin(2.0 * M_PI * 0.7 * t + 1.0);

        double left_val  = env_l * sin(2.0 * M_PI * 440.0 * t);
        double right_val = env_r * sin(2.0 * M_PI * 330.0 * t);

        /* Scale to int16 range and add noise */
        s_left_buf[i]  = (int16_t)(left_val  * 16000.0) + mock_noise();
        s_right_buf[i] = (int16_t)(right_val * 16000.0) + mock_noise();
    }

    frame->left         = s_left_buf;
    frame->right        = s_right_buf;
    frame->sample_count = PDM_MIC_FRAME_SAMPLES_PER_CHANNEL;

    s_frame_index++;
    return CY_RSLT_SUCCESS;
}

cy_rslt_t pdm_mic_deinit(void)
{
    s_initialized = false;
    printf("[MOCK][MIC] DEINIT\n");
    return CY_RSLT_SUCCESS;
}
