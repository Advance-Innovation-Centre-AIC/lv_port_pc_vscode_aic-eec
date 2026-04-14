/*******************************************************************************
 * layout_i18n.h — Health UI internationalization (TH/EN)
 *
 * Zero-copy string table in ROM. O(1) lookup by enum ID.
 * Memory: ~7 KB ROM, 4 bytes RAM.
 ******************************************************************************/

#ifndef HEALTH_LAYOUT_I18N_H
#define HEALTH_LAYOUT_I18N_H

#include <stdint.h>

/*******************************************************************************
 * Locale
 ******************************************************************************/
typedef enum {
    HEALTH_UI_LOCALE_EN = 0,
    HEALTH_UI_LOCALE_TH = 1,
} health_ui_locale_t;

/*******************************************************************************
 * String IDs — every user-visible text in the Health UI
 ******************************************************************************/
typedef enum {
    /* Navigation tabs */
    STR_TAB_HEALTH = 0,
    STR_TAB_HOME,
    STR_TAB_SETTING,
    STR_TAB_PRE_AUTH,
    STR_TAB_USER_DETAIL,

    /* Family */
    STR_FAMILY_NAME,

    /* Member names (10 members: grandparents → parents → children) */
    STR_MEMBER_0,
    STR_MEMBER_1,
    STR_MEMBER_2,
    STR_MEMBER_3,
    STR_MEMBER_4,
    STR_MEMBER_5,
    STR_MEMBER_6,
    STR_MEMBER_7,
    STR_MEMBER_8,
    STR_MEMBER_9,

    /* Metric names (full) */
    STR_METRIC_BP,
    STR_METRIC_GLUCOSE,
    STR_METRIC_TEMP,
    STR_METRIC_SPO2,
    STR_METRIC_WEIGHT,
    STR_METRIC_SLEEP,

    /* Metric names (short — for card headers) */
    STR_METRIC_BP_SHORT,
    STR_METRIC_GLUCOSE_SHORT,
    STR_METRIC_TEMP_SHORT,
    STR_METRIC_SPO2_SHORT,
    STR_METRIC_WEIGHT_SHORT,
    STR_METRIC_SLEEP_SHORT,

    /* Metric detail tab titles */
    STR_DETAIL_BP,
    STR_DETAIL_GLUCOSE,
    STR_DETAIL_TEMP,
    STR_DETAIL_SPO2,
    STR_DETAIL_WEIGHT,
    STR_DETAIL_SLEEP,

    /* Units */
    STR_UNIT_MMHG,
    STR_UNIT_MGDL,
    STR_UNIT_CELSIUS,
    STR_UNIT_PERCENT,
    STR_UNIT_KG,
    STR_UNIT_HR,
    STR_UNIT_BPM,

    /* BP sub-labels */
    STR_BP_SYS,
    STR_BP_DIA,
    STR_BP_PULSE,

    /* Status indicators */
    STR_STATUS_NORMAL,
    STR_STATUS_LOW_FEVER,
    STR_STATUS_HIGH_FEVER,
    STR_STATUS_PREDIABETES,
    STR_STATUS_HIGH,
    STR_STATUS_ELEVATED,
    STR_STATUS_CRITICAL,
    STR_STATUS_WARNING,
    STR_STATUS_LOW,

    /* Time-related */
    STR_TIME_JUST_NOW,
    STR_TIME_MIN_AGO,
    STR_TIME_HR_AGO,
    STR_TIME_DAYS_AGO,
    STR_TIME_NO_PREVIOUS,
    STR_TIME_PREVIOUS,

    /* Section headers */
    STR_HEALTH_INFORMATION,
    STR_HEALTH_RECORD,
    STR_DEVICES_LABEL,

    /* Device types (full name) */
    STR_DEVICE_BP_MONITOR,
    STR_DEVICE_GLUCOSE_METER,
    STR_DEVICE_THERMOMETER,
    STR_DEVICE_PULSE_OXIMETER,
    STR_DEVICE_WEIGHT_SCALE,

    /* Device abbreviations (avatar circle) */
    STR_DEVICE_ABBR_BP,
    STR_DEVICE_ABBR_BG,
    STR_DEVICE_ABBR_TH,
    STR_DEVICE_ABBR_SP,
    STR_DEVICE_ABBR_WT,

    /* Pairing flow */
    STR_PAIR_ADD_DEVICE,
    STR_PAIR_CONTINUE,
    STR_PAIR_NO_DEVICE,

    /* Chart periods */
    STR_PERIOD_7D,
    STR_PERIOD_30D,
    STR_PERIOD_90D,

    /* Misc */
    STR_DASHBOARD,
    STR_BACK,
    STR_AGE_PREFIX,

    STR_COUNT
} health_ui_str_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/** Initialize i18n system with default locale. */
void health_ui_i18n_init(health_ui_locale_t locale);

/** Switch locale at runtime. Call health_ui_root_refresh_strings() after. */
void health_ui_i18n_set_locale(health_ui_locale_t locale);

/** Get current locale. */
health_ui_locale_t health_ui_i18n_get_locale(void);

/** Look up a string by ID in the active locale. O(1). */
const char *health_ui_i18n_get(health_ui_str_t id);

/** Shorthand macro — use as: _(STR_TAB_HEALTH) */
#define _(id) health_ui_i18n_get(id)

#endif /* HEALTH_LAYOUT_I18N_H */
