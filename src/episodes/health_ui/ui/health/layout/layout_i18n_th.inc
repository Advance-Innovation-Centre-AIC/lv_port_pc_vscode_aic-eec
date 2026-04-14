/*******************************************************************************
 * layout_i18n_th.inc — Thai string table (ภาษาไทย)
 * Included by layout_i18n.c — do NOT compile directly.
 * Order MUST match health_ui_str_t enum in layout_i18n.h.
 ******************************************************************************/

static const char *const s_strings_th[STR_COUNT] = {
    /* Navigation tabs */
    [STR_TAB_HEALTH]        = "สุขภาพ",
    [STR_TAB_HOME]          = "หน้าแรก",
    [STR_TAB_SETTING]       = "ตั้งค่า",
    [STR_TAB_PRE_AUTH]      = "ลงทะเบียน",
    [STR_TAB_USER_DETAIL]   = "ข้อมูลสมาชิก",

    /* Family */
    [STR_FAMILY_NAME]       = "ครอบครัวสุขใจ",

    /* Member names (ชื่อเล่นไทย) */
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
    [STR_METRIC_BP]         = "ความดันโลหิต",
    [STR_METRIC_GLUCOSE]    = "น้ำตาลในเลือด",
    [STR_METRIC_TEMP]       = "อุณหภูมิร่างกาย",
    [STR_METRIC_SPO2]       = "ออกซิเจนในเลือด",
    [STR_METRIC_WEIGHT]     = "น้ำหนัก",
    [STR_METRIC_SLEEP]      = "การนอนหลับ",

    /* Metric names (short — for card headers) */
    [STR_METRIC_BP_SHORT]       = "ความดัน",
    [STR_METRIC_GLUCOSE_SHORT]  = "น้ำตาล",
    [STR_METRIC_TEMP_SHORT]     = "อุณหภูมิ",
    [STR_METRIC_SPO2_SHORT]     = "SpO2",
    [STR_METRIC_WEIGHT_SHORT]   = "น้ำหนัก",
    [STR_METRIC_SLEEP_SHORT]    = "การนอน",

    /* Metric detail tab titles */
    [STR_DETAIL_BP]         = "รายละเอียด ความดัน",
    [STR_DETAIL_GLUCOSE]    = "รายละเอียด น้ำตาล",
    [STR_DETAIL_TEMP]       = "รายละเอียด อุณหภูมิ",
    [STR_DETAIL_SPO2]       = "รายละเอียด SpO2",
    [STR_DETAIL_WEIGHT]     = "รายละเอียด น้ำหนัก",
    [STR_DETAIL_SLEEP]      = "รายละเอียด การนอน",

    /* Units */
    [STR_UNIT_MMHG]         = "mmHg",
    [STR_UNIT_MGDL]         = "mg/dL",
    [STR_UNIT_CELSIUS]      = "\xC2\xB0""C",
    [STR_UNIT_PERCENT]      = "%",
    [STR_UNIT_KG]           = "กก.",
    [STR_UNIT_HR]           = "ชม.",
    [STR_UNIT_BPM]          = "BPM",

    /* BP sub-labels */
    [STR_BP_SYS]            = "บน",
    [STR_BP_DIA]            = "ล่าง",
    [STR_BP_PULSE]          = "ชีพจร",

    /* Status indicators */
    [STR_STATUS_NORMAL]         = "ปกติ",
    [STR_STATUS_LOW_FEVER]      = "ไข้ต่ำ",
    [STR_STATUS_HIGH_FEVER]     = "ไข้สูง",
    [STR_STATUS_PREDIABETES]    = "เสี่ยงเบาหวาน",
    [STR_STATUS_HIGH]           = "สูง",
    [STR_STATUS_ELEVATED]       = "สูงเล็กน้อย",
    [STR_STATUS_CRITICAL]       = "วิกฤต",
    [STR_STATUS_WARNING]        = "เตือน",
    [STR_STATUS_LOW]            = "ต่ำ",

    /* Time-related */
    [STR_TIME_JUST_NOW]     = "เมื่อสักครู่",
    [STR_TIME_MIN_AGO]      = "นาทีที่แล้ว",
    [STR_TIME_HR_AGO]       = "ชม.ที่แล้ว",
    [STR_TIME_DAYS_AGO]     = "วันที่แล้ว",
    [STR_TIME_NO_PREVIOUS]  = "ไม่มีข้อมูลก่อน",
    [STR_TIME_PREVIOUS]     = "ค่าก่อนหน้า",

    /* Section headers */
    [STR_HEALTH_INFORMATION]= "ข้อมูลสุขภาพ",
    [STR_HEALTH_RECORD]     = "ประวัติสุขภาพ",
    [STR_DEVICES_LABEL]     = "อุปกรณ์",

    /* Device types */
    [STR_DEVICE_BP_MONITOR]     = "เครื่องวัดความดัน",
    [STR_DEVICE_GLUCOSE_METER]  = "เครื่องวัดน้ำตาล",
    [STR_DEVICE_THERMOMETER]    = "เทอร์มอมิเตอร์",
    [STR_DEVICE_PULSE_OXIMETER] = "เครื่องวัดออกซิเจน",
    [STR_DEVICE_WEIGHT_SCALE]   = "เครื่องชั่งน้ำหนัก",

    /* Device abbreviations (same in both) */
    [STR_DEVICE_ABBR_BP]    = "BP",
    [STR_DEVICE_ABBR_BG]    = "BG",
    [STR_DEVICE_ABBR_TH]    = "TH",
    [STR_DEVICE_ABBR_SP]    = "SP",
    [STR_DEVICE_ABBR_WT]    = "WT",

    /* Pairing flow */
    [STR_PAIR_ADD_DEVICE]   = "เพิ่มอุปกรณ์",
    [STR_PAIR_CONTINUE]     = "ต่อไป",
    [STR_PAIR_NO_DEVICE]    = "ไม่พบอุปกรณ์",

    /* Chart periods */
    [STR_PERIOD_7D]         = "7ว.",
    [STR_PERIOD_30D]        = "30ว.",
    [STR_PERIOD_90D]        = "90ว.",

    /* Misc */
    [STR_DASHBOARD]         = "แดชบอร์ด",
    [STR_BACK]              = "กลับ",
    [STR_AGE_PREFIX]        = "อายุ:",
};
