/*******************************************************************************
 * layout_i18n_en.inc — English string table
 * Included by layout_i18n.c — do NOT compile directly.
 * Order MUST match health_ui_str_t enum in layout_i18n.h.
 ******************************************************************************/

static const char *const s_strings_en[STR_COUNT] = {
    /* Navigation tabs */
    [STR_TAB_HEALTH]        = "Health",
    [STR_TAB_HOME]          = "Home",
    [STR_TAB_SETTING]       = "Setting",
    [STR_TAB_PRE_AUTH]      = "Pre-Auth",
    [STR_TAB_USER_DETAIL]   = "User Detail",

    /* Family */
    [STR_FAMILY_NAME]       = "Sukjai's Family",

    /* Member names */
    [STR_MEMBER_0]          = "ปู่มั่น",
    [STR_MEMBER_1]          = "ย่าทอง",
    [STR_MEMBER_2]          = "ตาชาม",
    [STR_MEMBER_3]          = "ยายผิว",
    [STR_MEMBER_4]          = "พ่อเอก",
    [STR_MEMBER_5]          = "แม่ปิ่น",
    [STR_MEMBER_6]          = "น้องโอม",
    [STR_MEMBER_7]          = "น้องมิ้นท์",
    [STR_MEMBER_8]          = "น้องปัน",
    [STR_MEMBER_9]          = "น้องนะโม",

    /* Metric names (full) */
    [STR_METRIC_BP]         = "Blood Pressure",
    [STR_METRIC_GLUCOSE]    = "Blood Glucose",
    [STR_METRIC_TEMP]       = "Body Temp",
    [STR_METRIC_SPO2]       = "SpO2",
    [STR_METRIC_WEIGHT]     = "Weight",
    [STR_METRIC_SLEEP]      = "Sleep Duration",

    /* Metric names (short) */
    [STR_METRIC_BP_SHORT]       = "Blood Pre",
    [STR_METRIC_GLUCOSE_SHORT]  = "Blood Glu",
    [STR_METRIC_TEMP_SHORT]     = "Body Temp",
    [STR_METRIC_SPO2_SHORT]     = "SpO2",
    [STR_METRIC_WEIGHT_SHORT]   = "Weight",
    [STR_METRIC_SLEEP_SHORT]    = "Sleep",

    /* Metric detail tab titles */
    [STR_DETAIL_BP]         = "BP Detail",
    [STR_DETAIL_GLUCOSE]    = "Glucose Detail",
    [STR_DETAIL_TEMP]       = "Temp Detail",
    [STR_DETAIL_SPO2]       = "SpO2 Detail",
    [STR_DETAIL_WEIGHT]     = "Weight Detail",
    [STR_DETAIL_SLEEP]      = "Sleep Detail",

    /* Units */
    [STR_UNIT_MMHG]         = "mmHg",
    [STR_UNIT_MGDL]         = "mg/dL",
    [STR_UNIT_CELSIUS]      = "\xC2\xB0""C",
    [STR_UNIT_PERCENT]      = "%",
    [STR_UNIT_KG]           = "kg",
    [STR_UNIT_HR]           = "hr",
    [STR_UNIT_BPM]          = "BPM",

    /* BP sub-labels */
    [STR_BP_SYS]            = "SYS",
    [STR_BP_DIA]            = "DIA",
    [STR_BP_PULSE]          = "PULSE",

    /* Status indicators */
    [STR_STATUS_NORMAL]         = "Normal",
    [STR_STATUS_LOW_FEVER]      = "Low fever",
    [STR_STATUS_HIGH_FEVER]     = "High fever",
    [STR_STATUS_PREDIABETES]    = "Prediabetes",
    [STR_STATUS_HIGH]           = "High",
    [STR_STATUS_ELEVATED]       = "Elevated",
    [STR_STATUS_CRITICAL]       = "Critical",
    [STR_STATUS_WARNING]        = "Warning",
    [STR_STATUS_LOW]            = "Low",

    /* Time-related */
    [STR_TIME_JUST_NOW]     = "Just now",
    [STR_TIME_MIN_AGO]      = "min ago",
    [STR_TIME_HR_AGO]       = "hr ago",
    [STR_TIME_DAYS_AGO]     = "days ago",
    [STR_TIME_NO_PREVIOUS]  = "No Previous",
    [STR_TIME_PREVIOUS]     = "Previous",

    /* Section headers */
    [STR_HEALTH_INFORMATION]= "Health information",
    [STR_HEALTH_RECORD]     = "Health Record",
    [STR_DEVICES_LABEL]     = "Devices",

    /* Device types */
    [STR_DEVICE_BP_MONITOR]     = "Blood Pressure Monitor",
    [STR_DEVICE_GLUCOSE_METER]  = "Blood Glucose Meter",
    [STR_DEVICE_THERMOMETER]    = "Thermometer",
    [STR_DEVICE_PULSE_OXIMETER] = "Pulse Oximeter",
    [STR_DEVICE_WEIGHT_SCALE]   = "Weight Scale",

    /* Device abbreviations */
    [STR_DEVICE_ABBR_BP]    = "BP",
    [STR_DEVICE_ABBR_BG]    = "BG",
    [STR_DEVICE_ABBR_TH]    = "TH",
    [STR_DEVICE_ABBR_SP]    = "SP",
    [STR_DEVICE_ABBR_WT]    = "WT",

    /* Pairing flow */
    [STR_PAIR_ADD_DEVICE]   = "Add Device",
    [STR_PAIR_CONTINUE]     = "Continue",
    [STR_PAIR_NO_DEVICE]    = "No pairable device.",

    /* Chart periods */
    [STR_PERIOD_7D]         = "7D",
    [STR_PERIOD_30D]        = "30D",
    [STR_PERIOD_90D]        = "90D",

    /* Misc */
    [STR_DASHBOARD]         = "Dashboard",
    [STR_BACK]              = "Back",
    [STR_AGE_PREFIX]        = "Age:",
};
