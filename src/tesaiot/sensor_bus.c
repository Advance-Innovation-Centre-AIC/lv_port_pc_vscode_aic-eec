/*******************************************************************************
 * @file    sensor_bus.c
 * @brief   PC simulator — define the dummy bus globals
 ******************************************************************************/
#include "sensor_bus.h"

mtb_hal_i2c_t sensor_i2c_controller_hal_obj;

static I3C_CORE_Type _i3c_hw_dummy;
I3C_CORE_Type *CYBSP_I3C_CONTROLLER_HW = &_i3c_hw_dummy;

cy_stc_i3c_context_t CYBSP_I3C_CONTROLLER_context;
