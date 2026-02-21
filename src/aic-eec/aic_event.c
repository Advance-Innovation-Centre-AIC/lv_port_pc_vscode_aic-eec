/*******************************************************************************
 * File: aic_event.c
 * Description: AIC-EEC Event Bus System Implementation (PC Simulator)
 *
 * Publish-Subscribe event system for decoupling sensor updates from UI.
 * Adapted for PC Simulator: no FreeRTOS, uses simple circular buffer.
 *
 * Part of BiiL Course: Embedded C for IoT
 ******************************************************************************/

#include "aic_event.h"
#include <string.h>
#include <stdio.h>

/*******************************************************************************
 * Type Definitions
 ******************************************************************************/

typedef struct {
    aic_event_cb_t callback;
    void *user_data;
} subscriber_t;

typedef struct {
    aic_event_t event;
    aic_event_data_t data;
    bool has_data;
} event_entry_t;

/*******************************************************************************
 * Static Variables
 ******************************************************************************/

static bool event_initialized = false;
static subscriber_t subscribers[AIC_EVENT_MAX][AIC_EVENT_MAX_SUBSCRIBERS];
static uint8_t subscriber_counts[AIC_EVENT_MAX];

/* Circular buffer replacing FreeRTOS queue */
static event_entry_t event_buffer[AIC_EVENT_QUEUE_SIZE];
static uint32_t buffer_head = 0;    /* Next write position */
static uint32_t buffer_tail = 0;    /* Next read position */
static uint32_t buffer_count = 0;   /* Number of items in buffer */

/*******************************************************************************
 * Helper Functions
 ******************************************************************************/

static void deliver_event(aic_event_t event, const aic_event_data_t *data)
{
    if (event >= AIC_EVENT_MAX) {
        return;
    }

    /* PC is single-threaded, no mutex needed */

    /* Call all subscribers for this event */
    for (uint8_t i = 0; i < subscriber_counts[event]; i++) {
        if (subscribers[event][i].callback != NULL) {
            subscribers[event][i].callback(event, data, subscribers[event][i].user_data);
        }
    }
}

/**
 * @brief Push an event entry into the circular buffer
 * @return true if pushed successfully, false if buffer full
 */
static bool buffer_push(const event_entry_t *entry)
{
    if (buffer_count >= AIC_EVENT_QUEUE_SIZE) {
        return false;  /* Buffer full */
    }

    memcpy(&event_buffer[buffer_head], entry, sizeof(event_entry_t));
    buffer_head = (buffer_head + 1) % AIC_EVENT_QUEUE_SIZE;
    buffer_count++;
    return true;
}

/**
 * @brief Pop an event entry from the circular buffer
 * @return true if popped successfully, false if buffer empty
 */
static bool buffer_pop(event_entry_t *entry)
{
    if (buffer_count == 0) {
        return false;  /* Buffer empty */
    }

    memcpy(entry, &event_buffer[buffer_tail], sizeof(event_entry_t));
    buffer_tail = (buffer_tail + 1) % AIC_EVENT_QUEUE_SIZE;
    buffer_count--;
    return true;
}

/*******************************************************************************
 * Public Functions
 ******************************************************************************/

bool aic_event_init(void)
{
    if (event_initialized) {
        return true;
    }

    /* Clear subscriber arrays */
    memset(subscribers, 0, sizeof(subscribers));
    memset(subscriber_counts, 0, sizeof(subscriber_counts));

    /* Initialize circular buffer */
    memset(event_buffer, 0, sizeof(event_buffer));
    buffer_head = 0;
    buffer_tail = 0;
    buffer_count = 0;

    event_initialized = true;
    printf("[EventBus] Initialized (PC Simulator, queue_size=%u)\n",
           (unsigned int)AIC_EVENT_QUEUE_SIZE);
    return true;
}

void aic_event_deinit(void)
{
    if (!event_initialized) {
        return;
    }

    /* Clear circular buffer */
    buffer_head = 0;
    buffer_tail = 0;
    buffer_count = 0;

    /* Clear all subscribers */
    memset(subscribers, 0, sizeof(subscribers));
    memset(subscriber_counts, 0, sizeof(subscriber_counts));

    event_initialized = false;
    printf("[EventBus] Deinitialized\n");
}

bool aic_event_is_init(void)
{
    return event_initialized;
}

bool aic_event_subscribe(aic_event_t event, aic_event_cb_t callback, void *user_data)
{
    if (event >= AIC_EVENT_MAX || callback == NULL) {
        return false;
    }

    if (!event_initialized) {
        return false;
    }

    /* Check if already subscribed */
    for (uint8_t i = 0; i < subscriber_counts[event]; i++) {
        if (subscribers[event][i].callback == callback) {
            /* Already subscribed, update user_data */
            subscribers[event][i].user_data = user_data;
            return true;
        }
    }

    /* Add new subscriber */
    if (subscriber_counts[event] < AIC_EVENT_MAX_SUBSCRIBERS) {
        subscribers[event][subscriber_counts[event]].callback = callback;
        subscribers[event][subscriber_counts[event]].user_data = user_data;
        subscriber_counts[event]++;
        return true;
    }

    return false;
}

bool aic_event_unsubscribe(aic_event_t event, aic_event_cb_t callback)
{
    if (event >= AIC_EVENT_MAX || callback == NULL) {
        return false;
    }

    if (!event_initialized) {
        return false;
    }

    for (uint8_t i = 0; i < subscriber_counts[event]; i++) {
        if (subscribers[event][i].callback == callback) {
            /* Found - shift remaining subscribers */
            for (uint8_t j = i; j < subscriber_counts[event] - 1; j++) {
                subscribers[event][j] = subscribers[event][j + 1];
            }
            subscriber_counts[event]--;

            /* Clear last slot */
            subscribers[event][subscriber_counts[event]].callback = NULL;
            subscribers[event][subscriber_counts[event]].user_data = NULL;

            return true;
        }
    }

    return false;
}

void aic_event_unsubscribe_all(aic_event_t event)
{
    if (event >= AIC_EVENT_MAX) {
        return;
    }

    if (!event_initialized) {
        return;
    }

    for (uint8_t i = 0; i < AIC_EVENT_MAX_SUBSCRIBERS; i++) {
        subscribers[event][i].callback = NULL;
        subscribers[event][i].user_data = NULL;
    }
    subscriber_counts[event] = 0;
}

bool aic_event_publish(aic_event_t event, const aic_event_data_t *data)
{
    if (event >= AIC_EVENT_MAX) {
        return false;
    }

    /* If no subscribers, don't bother queuing */
    if (subscriber_counts[event] == 0) {
        return true;  /* Success - nothing to do */
    }

    if (!event_initialized) {
        /* Not initialized - deliver immediately */
        deliver_event(event, data);
        return true;
    }

    event_entry_t entry = {
        .event = event,
        .has_data = (data != NULL)
    };

    if (data != NULL) {
        memcpy(&entry.data, data, sizeof(aic_event_data_t));
    }

    return buffer_push(&entry);
}

void aic_event_publish_immediate(aic_event_t event, const aic_event_data_t *data)
{
    if (event >= AIC_EVENT_MAX) {
        return;
    }

    deliver_event(event, data);
}

uint8_t aic_event_subscriber_count(aic_event_t event)
{
    if (event >= AIC_EVENT_MAX) {
        return 0;
    }
    return subscriber_counts[event];
}

uint32_t aic_event_queue_count(void)
{
    return buffer_count;
}

void aic_event_create_task(void)
{
    /* No-op on PC Simulator (no FreeRTOS tasks) */
    /* Events are processed via aic_event_process() called from main loop */
}

void aic_event_delete_task(void)
{
    /* No-op on PC Simulator */
}

void aic_event_process(void)
{
    if (!event_initialized) {
        return;
    }

    event_entry_t entry;
    while (buffer_pop(&entry)) {
        deliver_event(entry.event, entry.has_data ? &entry.data : NULL);
    }
}
