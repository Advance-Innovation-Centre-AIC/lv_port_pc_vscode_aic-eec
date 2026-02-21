/*******************************************************************************
 * File: aic_log.c
 * Description: AIC-EEC Logging System Implementation (PC Simulator)
 *
 * Adapted for PC Simulator: no FreeRTOS, no IPC.
 * Uses simple circular buffer instead of FreeRTOS queue.
 * Single-threaded: no mutex needed.
 *
 * Part of BiiL Course: Embedded C for IoT
 ******************************************************************************/

#include "aic_log.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/*******************************************************************************
 * Type Definitions
 ******************************************************************************/

typedef struct {
    aic_log_level_t level;
    char message[AIC_LOG_MSG_MAX_LEN];
} log_entry_t;

/*******************************************************************************
 * Static Variables
 ******************************************************************************/

static bool log_initialized = false;
static aic_log_level_t current_level = AIC_LOG_INFO;
static uint8_t output_targets = AIC_LOG_TARGET_PRINTF;

/* Circular buffer (replaces FreeRTOS queue) */
static log_entry_t log_buffer[AIC_LOG_QUEUE_SIZE];
static uint32_t buf_head = 0;    /* Next write position */
static uint32_t buf_tail = 0;    /* Next read position */
static uint32_t buf_count = 0;   /* Number of entries in buffer */

static uint32_t dropped_count = 0;

/* Level prefixes */
static const char *level_prefixes[] = {
    "",         /* NONE */
    "[E] ",     /* ERROR */
    "[W] ",     /* WARN */
    "[I] ",     /* INFO */
    "[D] ",     /* DEBUG */
    "[V] "      /* VERBOSE */
};

/* Level colors (ANSI escape codes) */
static const char *level_colors[] = {
    "",             /* NONE */
    "\033[31m",     /* ERROR - Red */
    "\033[33m",     /* WARN - Yellow */
    "\033[32m",     /* INFO - Green */
    "\033[36m",     /* DEBUG - Cyan */
    "\033[37m"      /* VERBOSE - White */
};

static const char *color_reset = "\033[0m";

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl/lvgl.h"
static lv_obj_t *lvgl_label = NULL;
static uint8_t lvgl_max_lines = 10;
static char lvgl_buffer[AIC_LOG_MSG_MAX_LEN * 10];
#endif

/*******************************************************************************
 * Circular Buffer Helpers
 ******************************************************************************/

static bool buf_push(const log_entry_t *entry)
{
    if (buf_count >= AIC_LOG_QUEUE_SIZE) {
        return false; /* Full */
    }
    log_buffer[buf_head] = *entry;
    buf_head = (buf_head + 1) % AIC_LOG_QUEUE_SIZE;
    buf_count++;
    return true;
}

static bool buf_pop(log_entry_t *entry)
{
    if (buf_count == 0) {
        return false; /* Empty */
    }
    *entry = log_buffer[buf_tail];
    buf_tail = (buf_tail + 1) % AIC_LOG_QUEUE_SIZE;
    buf_count--;
    return true;
}

/*******************************************************************************
 * Helper Functions
 ******************************************************************************/

static void output_message(const log_entry_t *entry)
{
    if (output_targets & AIC_LOG_TARGET_PRINTF) {
        printf("%s%s%s%s\n",
               level_colors[entry->level],
               level_prefixes[entry->level],
               entry->message,
               color_reset);
        fflush(stdout);
    }

    /* AIC_LOG_TARGET_IPC: no-op on PC Simulator (no IPC available) */

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    if ((output_targets & AIC_LOG_TARGET_LVGL) && lvgl_label != NULL) {
        /* Append to LVGL buffer */
        size_t current_len = strlen(lvgl_buffer);
        size_t msg_len = strlen(entry->message);

        /* Check if we need to scroll (remove first line) */
        if (lvgl_max_lines > 0) {
            int line_count = 0;
            for (size_t i = 0; i < current_len; i++) {
                if (lvgl_buffer[i] == '\n') line_count++;
            }

            while (line_count >= lvgl_max_lines && current_len > 0) {
                /* Find first newline and remove everything before it */
                char *newline = strchr(lvgl_buffer, '\n');
                if (newline != NULL) {
                    size_t remove_len = (newline - lvgl_buffer) + 1;
                    memmove(lvgl_buffer, newline + 1, current_len - remove_len + 1);
                    current_len -= remove_len;
                    line_count--;
                } else {
                    break;
                }
            }
        }

        /* Append new message */
        if (current_len + msg_len + 2 < sizeof(lvgl_buffer)) {
            if (current_len > 0) {
                strcat(lvgl_buffer, "\n");
            }
            strcat(lvgl_buffer, level_prefixes[entry->level]);
            strcat(lvgl_buffer, entry->message);

            lv_label_set_text(lvgl_label, lvgl_buffer);
        }
    }
#endif
}

/*******************************************************************************
 * Public Functions
 ******************************************************************************/

bool aic_log_init(void)
{
    if (log_initialized) {
        return true;
    }

    /* Initialize circular buffer */
    buf_head = 0;
    buf_tail = 0;
    buf_count = 0;

    log_initialized = true;
    dropped_count = 0;

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    lvgl_buffer[0] = '\0';
#endif

    return true;
}

void aic_log_deinit(void)
{
    if (!log_initialized) {
        return;
    }

    /* Drain remaining messages */
    aic_log_process();

    buf_head = 0;
    buf_tail = 0;
    buf_count = 0;

    log_initialized = false;
}

bool aic_log_is_init(void)
{
    return log_initialized;
}

void aic_log_set_level(aic_log_level_t level)
{
    current_level = level;
}

aic_log_level_t aic_log_get_level(void)
{
    return current_level;
}

void aic_log_set_targets(uint8_t targets)
{
    output_targets = targets;
}

uint8_t aic_log_get_targets(void)
{
    return output_targets;
}

void aic_log(aic_log_level_t level, const char *fmt, ...)
{
    /* Filter by level */
    if (level > current_level || level == AIC_LOG_NONE) {
        return;
    }

    /* If not initialized, output directly */
    if (!log_initialized) {
        va_list args;
        va_start(args, fmt);
        printf("%s", level_prefixes[level]);
        vprintf(fmt, args);
        printf("\n");
        fflush(stdout);
        va_end(args);
        return;
    }

    /* Create log entry */
    log_entry_t entry;
    entry.level = level;

    va_list args;
    va_start(args, fmt);
    vsnprintf(entry.message, AIC_LOG_MSG_MAX_LEN, fmt, args);
    va_end(args);

    /* If PRINTF target, output immediately (PC is single-threaded) */
    if (output_targets & AIC_LOG_TARGET_PRINTF) {
        printf("%s%s%s%s\n",
               level_colors[entry.level],
               level_prefixes[entry.level],
               entry.message,
               color_reset);
        fflush(stdout);
    }

    /* Queue for LVGL output (processed in main loop via aic_log_process) */
    if (!buf_push(&entry)) {
        dropped_count++;
    }
}

void aic_log_tag(aic_log_level_t level, const char *tag, const char *fmt, ...)
{
    /* Filter by level */
    if (level > current_level || level == AIC_LOG_NONE) {
        return;
    }

    /* If not initialized, output directly */
    if (!log_initialized) {
        va_list args;
        va_start(args, fmt);
        printf("%s[%s] ", level_prefixes[level], tag);
        vprintf(fmt, args);
        printf("\n");
        fflush(stdout);
        va_end(args);
        return;
    }

    /* Create log entry with tag */
    log_entry_t entry;
    entry.level = level;

    /* Format: [TAG] message */
    int tag_len = snprintf(entry.message, AIC_LOG_MSG_MAX_LEN, "[%s] ", tag);

    va_list args;
    va_start(args, fmt);
    vsnprintf(entry.message + tag_len, AIC_LOG_MSG_MAX_LEN - tag_len, fmt, args);
    va_end(args);

    /* If PRINTF target, output immediately (PC is single-threaded) */
    if (output_targets & AIC_LOG_TARGET_PRINTF) {
        printf("%s%s%s%s\n",
               level_colors[entry.level],
               level_prefixes[entry.level],
               entry.message,
               color_reset);
        fflush(stdout);
    }

    /* Queue for LVGL output (processed in main loop via aic_log_process) */
    if (!buf_push(&entry)) {
        dropped_count++;
    }
}

void aic_log_flush(void)
{
    if (!log_initialized) {
        return;
    }

    /* Drain all buffered messages */
    aic_log_process();
}

uint32_t aic_log_queue_count(void)
{
    return buf_count;
}

uint32_t aic_log_dropped_count(void)
{
    return dropped_count;
}

void aic_log_create_task(void)
{
    /* No-op on PC Simulator (no FreeRTOS) */
}

void aic_log_delete_task(void)
{
    /* No-op on PC Simulator (no FreeRTOS) */
}

void aic_log_process(void)
{
    if (!log_initialized) {
        return;
    }

    log_entry_t entry;
    while (buf_pop(&entry)) {
        /* output_message handles LVGL target; skip printf since already output */
#ifdef LV_LVGL_H_INCLUDE_SIMPLE
        if ((output_targets & AIC_LOG_TARGET_LVGL) && lvgl_label != NULL) {
            /* Append to LVGL buffer */
            size_t current_len = strlen(lvgl_buffer);
            size_t msg_len = strlen(entry.message);

            /* Check if we need to scroll (remove first line) */
            if (lvgl_max_lines > 0) {
                int line_count = 0;
                for (size_t i = 0; i < current_len; i++) {
                    if (lvgl_buffer[i] == '\n') line_count++;
                }

                while (line_count >= lvgl_max_lines && current_len > 0) {
                    char *newline = strchr(lvgl_buffer, '\n');
                    if (newline != NULL) {
                        size_t remove_len = (newline - lvgl_buffer) + 1;
                        memmove(lvgl_buffer, newline + 1, current_len - remove_len + 1);
                        current_len -= remove_len;
                        line_count--;
                    } else {
                        break;
                    }
                }
            }

            /* Append new message */
            if (current_len + msg_len + 2 < sizeof(lvgl_buffer)) {
                if (current_len > 0) {
                    strcat(lvgl_buffer, "\n");
                }
                strcat(lvgl_buffer, level_prefixes[entry.level]);
                strcat(lvgl_buffer, entry.message);

                lv_label_set_text(lvgl_label, lvgl_buffer);
            }
        }
#endif
    }
}

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
void aic_log_set_lvgl_label(lv_obj_t *label, uint8_t max_lines)
{
    lvgl_label = label;
    lvgl_max_lines = max_lines;
    lvgl_buffer[0] = '\0';

    if (label != NULL) {
        output_targets |= AIC_LOG_TARGET_LVGL;
    } else {
        output_targets &= ~AIC_LOG_TARGET_LVGL;
    }
}
#endif
