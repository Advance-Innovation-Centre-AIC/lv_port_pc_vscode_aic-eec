/*******************************************************************************
 * @file    cy_result.h
 * @brief   PC simulator stub for Infineon cy_result.h
 *
 *          Provides cy_rslt_t and common result codes so that episode headers
 *          that #include "cy_result.h" compile on PC without real PDL.
 ******************************************************************************/
#ifndef CY_RESULT_H
#define CY_RESULT_H

#include <stdint.h>

typedef uint32_t cy_rslt_t;

#define CY_RSLT_SUCCESS         ((cy_rslt_t)0x00000000U)
#define CY_RSLT_TYPE_ERROR      ((cy_rslt_t)0x00000001U)

/* CY_RSLT_CREATE macro used by some service files */
#ifndef CY_RSLT_CREATE
#define CY_RSLT_CREATE(type, module, code) \
    ((cy_rslt_t)( ((type) << 28U) | ((module) << 16U) | (code) ))
#endif

#endif /* CY_RESULT_H */
