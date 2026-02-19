/*******************************************************************************
 * File Name:   sensors.c
 *
 * Description: AIC-EEC Sensors API - PC Simulator Mock Implementation
 *
 ******************************************************************************/

#include "sensors.h"
#include <stdio.h>
#include <math.h>

/*******************************************************************************
 * Static Variables (Mock State)
 ******************************************************************************/

static uint8_t adc_values[AIC_ADC_COUNT] = {50, 25, 75};  /* Default values */
static aic_imu_data_t imu_data = {0, 0, 16384, 0, 0, 0};  /* Default: Z = 1g */
static uint32_t sim_tick = 0;
static bool imu_data_ready = true;

/*******************************************************************************
 * Sensor Functions
 ******************************************************************************/

void aic_sensors_init(void)
{
    printf("[Sensors Mock] Initialized\n");
    printf("  - ADC CH0 (POTEN): Use slider or mouse to control\n");
    printf("  - IMU: Generates animated sine wave data\n");
}

uint8_t aic_adc_read_percent(aic_adc_channel_t channel)
{
    if (channel < AIC_ADC_COUNT) {
        return adc_values[channel];
    }
    return 0;
}

uint16_t aic_adc_read_raw(aic_adc_channel_t channel)
{
    if (channel < AIC_ADC_COUNT) {
        return (uint16_t)(adc_values[channel] * 4095 / 100);
    }
    return 0;
}

uint16_t aic_adc_read_mv(aic_adc_channel_t channel)
{
    if (channel < AIC_ADC_COUNT) {
        return (uint16_t)(adc_values[channel] * 3300 / 100);
    }
    return 0;
}

bool aic_imu_data_available(void)
{
    return imu_data_ready;
}

bool aic_sensors_get_imu(aic_imu_data_t *data)
{
    if (data) {
        *data = imu_data;
        return true;
    }
    return false;
}

bool aic_imu_read_accel(int16_t *ax, int16_t *ay, int16_t *az)
{
    /* Update simulated data */
    aic_sensors_sim_tick();

    if (ax) *ax = imu_data.accel_x;
    if (ay) *ay = imu_data.accel_y;
    if (az) *az = imu_data.accel_z;
    return true;
}

bool aic_imu_read_gyro(int16_t *gx, int16_t *gy, int16_t *gz)
{
    if (gx) *gx = imu_data.gyro_x;
    if (gy) *gy = imu_data.gyro_y;
    if (gz) *gz = imu_data.gyro_z;
    return true;
}

/*******************************************************************************
 * Simulator-Specific Functions
 ******************************************************************************/

void aic_sensors_sim_set_adc(aic_adc_channel_t channel, uint8_t percent)
{
    if (channel < AIC_ADC_COUNT) {
        if (percent > 100) percent = 100;
        adc_values[channel] = percent;
    }
}

void aic_sensors_sim_set_imu(const aic_imu_data_t *data)
{
    if (data) {
        imu_data = *data;
    }
}

void aic_sensors_sim_tick(void)
{
    sim_tick++;

    /* Generate animated IMU data using sine waves */
    float t = (float)sim_tick * 0.05f;  /* Time factor */

    /* Simulate gentle tilting motion */
    float roll_angle = sinf(t * 0.3f) * 15.0f;   /* Roll: ±15 degrees */
    float pitch_angle = sinf(t * 0.2f) * 10.0f;  /* Pitch: ±10 degrees */

    /* Convert angles to accelerometer values (simplified) */
    /* At rest: Z = 1g (16384), X and Y depend on tilt */
    float roll_rad = roll_angle * 3.14159f / 180.0f;
    float pitch_rad = pitch_angle * 3.14159f / 180.0f;

    imu_data.accel_x = (int16_t)(sinf(roll_rad) * 16384.0f);
    imu_data.accel_y = (int16_t)(sinf(pitch_rad) * 16384.0f);
    imu_data.accel_z = (int16_t)(cosf(roll_rad) * cosf(pitch_rad) * 16384.0f);

    /* Simulate gyroscope (angular velocity) */
    imu_data.gyro_x = (int16_t)(cosf(t * 0.3f) * 500.0f);
    imu_data.gyro_y = (int16_t)(cosf(t * 0.2f) * 400.0f);
    imu_data.gyro_z = (int16_t)(sinf(t * 0.1f) * 200.0f);

    imu_data_ready = true;
}
