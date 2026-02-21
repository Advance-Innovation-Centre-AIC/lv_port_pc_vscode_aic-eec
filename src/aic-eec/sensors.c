/*******************************************************************************
 * File Name:   sensors.c
 *
 * Description: AIC-EEC Sensor Hardware Abstraction Layer
 *              PC Simulator Mock Implementation
 *
 *              All sensor reads return simulated data.
 *              ADC values can be set externally; IMU generates sine waves.
 *
 ******************************************************************************/

#include "sensors.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

/*******************************************************************************
 * Static Variables (Mock State)
 ******************************************************************************/

static bool initialized = false;
static bool simulation_mode = true;     /* PC is always simulation */

static uint16_t adc_values[AIC_ADC_CH_COUNT];  /* raw 12-bit values */
static uint16_t vref_mv = 3300;
static aic_adc_resolution_t adc_resolution = AIC_ADC_RES_12BIT;

static aic_imu_data_t imu_data;
static float sim_accel[3] = {0.0f, 0.0f, 1.0f};  /* flat on table */
static float sim_gyro[3]  = {0.0f, 0.0f, 0.0f};

static uint32_t sim_tick_count = 0;

/*******************************************************************************
 * Helper: max raw value for current resolution
 ******************************************************************************/

static uint16_t adc_max_val(void)
{
    return (uint16_t)((1 << (int)adc_resolution) - 1);
}

/*******************************************************************************
 * Sensor Initialization
 ******************************************************************************/

bool aic_sensors_init(void)
{
    /* Set default ADC values (12-bit scale) */
    adc_values[AIC_ADC_CH0]  = 2048;   /* ~50% */
    adc_values[AIC_ADC_CH1]  = 1024;   /* ~25% */
    adc_values[AIC_ADC_CH2]  = 3072;   /* ~75% */
    adc_values[AIC_ADC_CH3]  = 512;    /* ~12.5% */
    adc_values[AIC_ADC_TEMP] = 1500;   /* ~25 C baseline */

    adc_resolution = AIC_ADC_RES_12BIT;
    vref_mv = 3300;

    sim_accel[0] = 0.0f;
    sim_accel[1] = 0.0f;
    sim_accel[2] = 1.0f;
    sim_gyro[0]  = 0.0f;
    sim_gyro[1]  = 0.0f;
    sim_gyro[2]  = 0.0f;

    memset(&imu_data, 0, sizeof(imu_data));
    imu_data.accel_z = 1.0f;
    imu_data.accel_raw_z = 1000;

    sim_tick_count = 0;
    simulation_mode = true;
    initialized = true;

    printf("[Sensors] Initialized (simulation mode)\n");
    return true;
}

void aic_sensors_deinit(void)
{
    initialized = false;
    printf("[Sensors] Deinitialized\n");
}

/*******************************************************************************
 * ADC Functions
 ******************************************************************************/

bool aic_adc_init(aic_adc_resolution_t resolution)
{
    adc_resolution = resolution;
    printf("[ADC] Initialized, resolution=%d-bit\n", (int)resolution);
    return true;
}

uint16_t aic_adc_read(aic_adc_channel_t channel)
{
    if (channel >= AIC_ADC_CH_COUNT) {
        return 0;
    }

    /* adc_values[] stored as 12-bit; scale to current resolution */
    uint16_t raw12 = adc_values[channel];
    if (adc_resolution == AIC_ADC_RES_12BIT) {
        return raw12;
    }
    /* Scale: raw12 / 4095 * max_val */
    return (uint16_t)((uint32_t)raw12 * adc_max_val() / 4095);
}

float aic_adc_read_voltage(aic_adc_channel_t channel)
{
    uint16_t raw = aic_adc_read(channel);
    uint16_t max_v = adc_max_val();
    if (max_v == 0) {
        return 0.0f;
    }
    return (float)raw * (float)vref_mv / (float)max_v / 1000.0f;
}

uint8_t aic_adc_read_percent(aic_adc_channel_t channel)
{
    uint16_t raw = aic_adc_read(channel);
    uint16_t max_v = adc_max_val();
    if (max_v == 0) {
        return 0;
    }
    return (uint8_t)((uint32_t)raw * 100 / max_v);
}

void aic_adc_set_vref(uint16_t new_vref_mv)
{
    vref_mv = new_vref_mv;
}

uint16_t aic_adc_get_vref(void)
{
    return vref_mv;
}

float aic_adc_read_temperature(void)
{
    /* Simulated: baseline 25 C at raw 1500, 0.01 C per count */
    return 25.0f + (float)((int)adc_values[AIC_ADC_TEMP] - 1500) * 0.01f;
}

/*******************************************************************************
 * IMU Functions
 ******************************************************************************/

bool aic_imu_init(void)
{
    printf("[IMU] Initialized (simulation mode)\n");
    return true;
}

bool aic_imu_read_accel(float *ax, float *ay, float *az)
{
    if (ax) *ax = sim_accel[0];
    if (ay) *ay = sim_accel[1];
    if (az) *az = sim_accel[2];
    return true;
}

bool aic_imu_read_gyro(float *gx, float *gy, float *gz)
{
    if (gx) *gx = sim_gyro[0];
    if (gy) *gy = sim_gyro[1];
    if (gz) *gz = sim_gyro[2];
    return true;
}

bool aic_imu_read_all(aic_imu_data_t *data)
{
    if (!data) {
        return false;
    }

    data->accel_x = sim_accel[0];
    data->accel_y = sim_accel[1];
    data->accel_z = sim_accel[2];
    data->gyro_x  = sim_gyro[0];
    data->gyro_y  = sim_gyro[1];
    data->gyro_z  = sim_gyro[2];

    data->accel_raw_x = (int32_t)(sim_accel[0] * 1000.0f);
    data->accel_raw_y = (int32_t)(sim_accel[1] * 1000.0f);
    data->accel_raw_z = (int32_t)(sim_accel[2] * 1000.0f);
    data->gyro_raw_x  = (int32_t)(sim_gyro[0] * 1000.0f);
    data->gyro_raw_y  = (int32_t)(sim_gyro[1] * 1000.0f);
    data->gyro_raw_z  = (int32_t)(sim_gyro[2] * 1000.0f);

    return true;
}

aic_orientation_t aic_imu_get_orientation(void)
{
    float ax = sim_accel[0];
    float ay = sim_accel[1];
    float az = sim_accel[2];

    /* Dominant axis determines orientation */
    if (fabsf(az) > 0.7f) {
        return (az > 0.0f) ? AIC_ORIENT_FLAT_UP : AIC_ORIENT_FLAT_DOWN;
    }
    if (fabsf(ax) > 0.7f) {
        return (ax > 0.0f) ? AIC_ORIENT_LANDSCAPE : AIC_ORIENT_LANDSCAPE_INV;
    }
    if (fabsf(ay) > 0.7f) {
        return (ay > 0.0f) ? AIC_ORIENT_PORTRAIT : AIC_ORIENT_PORTRAIT_INV;
    }
    return AIC_ORIENT_UNKNOWN;
}

const char* aic_imu_orientation_name(aic_orientation_t orient)
{
    switch (orient) {
        case AIC_ORIENT_FLAT_UP:       return "Flat Up";
        case AIC_ORIENT_FLAT_DOWN:     return "Flat Down";
        case AIC_ORIENT_PORTRAIT:      return "Portrait";
        case AIC_ORIENT_PORTRAIT_INV:  return "Portrait Inv";
        case AIC_ORIENT_LANDSCAPE:     return "Landscape";
        case AIC_ORIENT_LANDSCAPE_INV: return "Landscape Inv";
        default:                       return "Unknown";
    }
}

bool aic_imu_calibrate(void)
{
    printf("[IMU] Calibration complete (simulated)\n");
    return true;
}

bool aic_imu_is_available(void)
{
    return true;  /* Always available in simulation */
}

/*******************************************************************************
 * CAPSENSE Functions (not available on PC)
 ******************************************************************************/

bool aic_capsense_init(void)
{
    printf("[CAPSENSE] Not available on PC Simulator\n");
    return false;
}

bool aic_capsense_read(aic_capsense_data_t *data)
{
    (void)data;
    return false;
}

bool aic_capsense_is_available(void)
{
    return false;
}

/*******************************************************************************
 * Simulation Functions
 ******************************************************************************/

void aic_sensors_set_simulation(bool enable)
{
    /* PC simulator is always in simulation mode */
    simulation_mode = enable;
    (void)enable;
}

bool aic_sensors_is_simulation(void)
{
    return simulation_mode;
}

void aic_adc_set_simulated(aic_adc_channel_t channel, uint16_t value)
{
    if (channel < AIC_ADC_CH_COUNT) {
        adc_values[channel] = value;
    }
}

void aic_imu_set_simulated_accel(float ax, float ay, float az)
{
    sim_accel[0] = ax;
    sim_accel[1] = ay;
    sim_accel[2] = az;
}

void aic_imu_set_simulated_gyro(float gx, float gy, float gz)
{
    sim_gyro[0] = gx;
    sim_gyro[1] = gy;
    sim_gyro[2] = gz;
}

/*******************************************************************************
 * Utility Functions
 ******************************************************************************/

const char* aic_adc_channel_name(aic_adc_channel_t channel)
{
    switch (channel) {
        case AIC_ADC_CH0:  return "CH0 (Pot)";
        case AIC_ADC_CH1:  return "CH1";
        case AIC_ADC_CH2:  return "CH2";
        case AIC_ADC_CH3:  return "CH3";
        case AIC_ADC_TEMP: return "Temp";
        default:           return "Unknown";
    }
}

float aic_adc_raw_to_voltage(uint16_t raw, aic_adc_resolution_t resolution,
                             uint16_t ref_mv)
{
    uint16_t max_v = (uint16_t)((1 << (int)resolution) - 1);
    if (max_v == 0) {
        return 0.0f;
    }
    return (float)raw * (float)ref_mv / (float)max_v / 1000.0f;
}

void aic_sensors_print_status(void)
{
    printf("[Sensors] Status:\n");
    printf("  Mode: %s\n", simulation_mode ? "Simulation" : "Hardware");
    printf("  ADC resolution: %d-bit, VREF: %u mV\n",
           (int)adc_resolution, (unsigned int)vref_mv);

    for (int i = 0; i < AIC_ADC_CH_COUNT; i++) {
        printf("  ADC %s: raw=%u, %u%%, %.3f V\n",
               aic_adc_channel_name((aic_adc_channel_t)i),
               (unsigned int)adc_values[i],
               (unsigned int)aic_adc_read_percent((aic_adc_channel_t)i),
               aic_adc_read_voltage((aic_adc_channel_t)i));
    }

    printf("  Temperature: %.1f C\n", aic_adc_read_temperature());
    printf("  Accel: X=%.3f Y=%.3f Z=%.3f g\n",
           sim_accel[0], sim_accel[1], sim_accel[2]);
    printf("  Gyro:  X=%.1f Y=%.1f Z=%.1f deg/s\n",
           sim_gyro[0], sim_gyro[1], sim_gyro[2]);
    printf("  Orientation: %s\n",
           aic_imu_orientation_name(aic_imu_get_orientation()));
    printf("  CAPSENSE: not available\n");
}

/*******************************************************************************
 * PC Simulator Only
 ******************************************************************************/

void aic_sensors_sim_set_adc(aic_adc_channel_t channel, uint8_t percent)
{
    if (channel >= AIC_ADC_CH_COUNT) {
        return;
    }
    if (percent > 100) {
        percent = 100;
    }
    /* Store as 12-bit raw regardless of current resolution */
    adc_values[channel] = (uint16_t)((uint32_t)percent * 4095 / 100);
}

void aic_sensors_sim_set_imu(const aic_imu_data_t *data)
{
    if (!data) {
        return;
    }
    sim_accel[0] = data->accel_x;
    sim_accel[1] = data->accel_y;
    sim_accel[2] = data->accel_z;
    sim_gyro[0]  = data->gyro_x;
    sim_gyro[1]  = data->gyro_y;
    sim_gyro[2]  = data->gyro_z;
}

void aic_sensors_sim_tick(void)
{
    sim_tick_count++;
    float t = (float)sim_tick_count;

    /* Gentle animated motion */
    /* Roll:  sin wave on X-axis, +/-0.25 g */
    float roll  = sinf(t * 0.03f) * 0.25f;
    /* Pitch: sin wave on Y-axis, +/-0.17 g */
    float pitch = sinf(t * 0.02f) * 0.17f;
    /* Z stays near 1.0 g (gravity), slight variation from tilt */
    float z_comp = sqrtf(1.0f - roll * roll - pitch * pitch);
    if (z_comp < 0.5f) {
        z_comp = 0.5f;  /* Clamp for safety */
    }

    sim_accel[0] = roll;
    sim_accel[1] = pitch;
    sim_accel[2] = z_comp;

    /* Gyro: approximate derivatives of the accel sine waves */
    /* d/dt sin(t*0.03)*0.25 = cos(t*0.03)*0.03*0.25 -> scale to deg/s */
    sim_gyro[0] = cosf(t * 0.03f) * 0.03f * 0.25f * 500.0f;  /* ~3.75 deg/s peak */
    sim_gyro[1] = cosf(t * 0.02f) * 0.02f * 0.17f * 500.0f;  /* ~1.70 deg/s peak */
    sim_gyro[2] = sinf(t * 0.01f) * 1.0f;                     /* slow yaw drift */
}
