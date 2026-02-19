/*******************************************************************************
 * File Name:   sensors.h
 *
 * Description: AIC-EEC Sensors API - PC Simulator Mock Version
 *              For LVGL UI Testing on MacOS/Windows/Linux
 *
 * Note: This is a mock implementation for PC simulation.
 *       ADC values can be set via slider/mouse, IMU generates sine waves.
 *
 ******************************************************************************/

#ifndef AIC_SENSORS_H
#define AIC_SENSORS_H

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * ADC Channel Definitions
 ******************************************************************************/

typedef enum {
    AIC_ADC_CH0 = 0,    /**< Potentiometer */
    AIC_ADC_CH1,        /**< Temperature sensor */
    AIC_ADC_CH2,        /**< Light sensor */
    AIC_ADC_COUNT
} aic_adc_channel_t;

/*******************************************************************************
 * IMU Data Structure
 ******************************************************************************/

typedef struct {
    int16_t accel_x;    /**< Accelerometer X (-32768 to 32767) */
    int16_t accel_y;    /**< Accelerometer Y */
    int16_t accel_z;    /**< Accelerometer Z */
    int16_t gyro_x;     /**< Gyroscope X */
    int16_t gyro_y;     /**< Gyroscope Y */
    int16_t gyro_z;     /**< Gyroscope Z */
} aic_imu_data_t;

/*******************************************************************************
 * Sensor Functions (Mock Implementation)
 ******************************************************************************/

/**
 * @brief Initialize sensors (no-op on PC)
 */
void aic_sensors_init(void);

/**
 * @brief Read ADC value as percentage
 * @param channel ADC channel
 * @return Value 0-100%
 */
uint8_t aic_adc_read_percent(aic_adc_channel_t channel);

/**
 * @brief Read raw ADC value
 * @param channel ADC channel
 * @return Raw value 0-4095
 */
uint16_t aic_adc_read_raw(aic_adc_channel_t channel);

/**
 * @brief Read ADC as voltage
 * @param channel ADC channel
 * @return Voltage in millivolts (0-3300)
 */
uint16_t aic_adc_read_mv(aic_adc_channel_t channel);

/**
 * @brief Check if new IMU data is available
 * @return true if new data available
 */
bool aic_imu_data_available(void);

/**
 * @brief Get latest IMU data
 * @param data Pointer to IMU data structure
 * @return true if successful
 */
bool aic_sensors_get_imu(aic_imu_data_t *data);

/**
 * @brief Read accelerometer data
 * @param ax Pointer to X acceleration
 * @param ay Pointer to Y acceleration
 * @param az Pointer to Z acceleration
 * @return true if successful
 */
bool aic_imu_read_accel(int16_t *ax, int16_t *ay, int16_t *az);

/**
 * @brief Read gyroscope data
 * @param gx Pointer to X angular velocity
 * @param gy Pointer to Y angular velocity
 * @param gz Pointer to Z angular velocity
 * @return true if successful
 */
bool aic_imu_read_gyro(int16_t *gx, int16_t *gy, int16_t *gz);

/*******************************************************************************
 * Simulator-Specific Functions
 ******************************************************************************/

/**
 * @brief Set simulated ADC value (for UI control)
 * @param channel ADC channel
 * @param percent Value 0-100%
 */
void aic_sensors_sim_set_adc(aic_adc_channel_t channel, uint8_t percent);

/**
 * @brief Set simulated IMU data (for UI control)
 * @param data Pointer to IMU data
 */
void aic_sensors_sim_set_imu(const aic_imu_data_t *data);

/**
 * @brief Update simulation tick (call periodically to animate IMU)
 */
void aic_sensors_sim_tick(void);

#endif /* AIC_SENSORS_H */
