/*******************************************************************************
 * @file    sensor_bus.h
 * @brief   PC simulator stubs for hardware bus types and globals
 ******************************************************************************/
#ifndef SENSOR_BUS_H
#define SENSOR_BUS_H

/* PC simulator stubs — no real hardware */
typedef void* mtb_hal_i2c_t;
typedef void* I3C_CORE_Type;
typedef void* cy_stc_i3c_context_t;

#include "cy_result.h"

/* Dummy globals that episode code references */
extern mtb_hal_i2c_t sensor_i2c_controller_hal_obj;
extern I3C_CORE_Type *CYBSP_I3C_CONTROLLER_HW;
extern cy_stc_i3c_context_t CYBSP_I3C_CONTROLLER_context;

#endif /* SENSOR_BUS_H */
