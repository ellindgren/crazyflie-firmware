/**
 * Förslag för att göra det mer robust:
 * Ha en flagga som sätts i __position_cb
 * Sätts till 1 varje gång ett nytt vörde läses in
 * Sätts till 0 varje gång ett värde sparas
 */
#include "aideck_protocol.h"

#ifdef VERBOSE_RX
static uint8_t verbose_rx = 0;
#endif

#define REQUEST_TIMEOUT 2000 // number of milliseconds to wait for a confirmation

#define AIDECK_HAS_CONFIGS

// --- config camera

void log_camera(camera_t *value)
{
    DEBUG_PRINT("marginTop=%u, marginRight=%u, marginBottom=%u, marginLeft=%u, format=%u, step=%u, target_value=%u, ae=%u, fps=%u\n", value->marginTop, value->marginRight, value->marginBottom, value->marginLeft, value->format, value->step, value->target_value, value->ae, value->fps);
}

static struct
{
    uint32_t request_time;
    camera_t value, dvalue;
    const char *header;
} __camera__config = {.request_time = 0, .header = "!CAM"};

void __camera_cb(void *buffer)
{
    camera_t *value = (camera_t *)buffer;
    __camera__config.value = *value;
#ifdef VERBOSE_RX
    if (verbose_rx)
    {
        uint32_t now = T2M(xTaskGetTickCount());
        if (__camera__config.request_time && memcmp(&(__camera__config.value), &(__camera__config.dvalue), sizeof(camera_t)))
        {
            DEBUG_PRINT("[WARNING] GAP has set camera config after %ld ms\n", now - __camera__config.request_time);
            log_camera(&(__camera__config.value));
            DEBUG_PRINT("to a different value than the one requested\n");
            log_camera(&(__camera__config.dvalue));
        }
        else
        {
            if (__camera__config.request_time)
                DEBUG_PRINT("GAP has set camera config after %ld ms to\n", now - __camera__config.request_time);
            else
                DEBUG_PRINT("GAP has set camera config to\n");
            
            log_camera(&(__camera__config.value));
        }
    }
#endif // VERBOSE_RX
    __camera__config.dvalue = __camera__config.value;
    __camera__config.request_time = 0;
}

// --- config stream

void log_stream(stream_t *value)
{
    DEBUG_PRINT("on=%u, format=%u, transport=%u\n", value->on, value->format, value->transport);
}

static struct
{
    uint32_t request_time;
    stream_t value, dvalue;
    const char *header;
} __stream__config = {.request_time = 0, .header = "!STR"};

void __stream_cb(void *buffer)
{
    stream_t *value = (stream_t *)buffer;
    __stream__config.value = *value;

#ifdef VERBOSE_RX
    if (verbose_rx)
    {
        uint32_t now = T2M(xTaskGetTickCount());

        if (__stream__config.request_time && memcmp(&(__stream__config.value), &(__stream__config.dvalue), sizeof(stream_t)))
        {
            DEBUG_PRINT("[WARNING] GAP has set stream config after %ld ms\n", now - __stream__config.request_time);
            log_stream(&(__stream__config.value));
            DEBUG_PRINT("to a different value than the one requested\n");
            log_stream(&(__stream__config.dvalue));
        }
        else
        {
            if (__stream__config.request_time)
                DEBUG_PRINT("GAP has set stream config after %ld ms to\n", now - __stream__config.request_time);
            else
                DEBUG_PRINT("GAP has set stream config to\n");

            log_stream(&(__stream__config.value));
        }
    }
#endif // VERBOSE_RX
    __stream__config.dvalue = __stream__config.value;
    __stream__config.request_time = 0;
}

void save_stream_values(stream_t *savehere)
{
    savehere->transport = __stream__config.value.transport;
    savehere->format = __stream__config.value.format;
    savehere->on = __stream__config.value.on;
}



// --- config position

void log_position(position_t *value)
{
    DEBUG_PRINT("xc=%d, yc=%d, height=%d, width=%d\n", value->xc, value->yc, value->height, value->width);
}

static struct
{
    uint32_t request_time;
    position_t value, dvalue;
    const char *header;
} __position__config = {.request_time = 0, .header = "!POS"};

void __position_cb(void *buffer)
{
    position_t *value = (position_t *)buffer;
    __position__config.value = *value;

#ifdef VERBOSE_RX
    if (verbose_rx)
    {
        uint32_t now = T2M(xTaskGetTickCount());

        if (__position__config.request_time && memcmp(&(__position__config.value), &(__position__config.dvalue), sizeof(position_t)))
        {
            DEBUG_PRINT("[WARNING] GAP has set position config after %ld ms\n", now - __position__config.request_time);
            log_position(&(__position__config.value));
            DEBUG_PRINT("to a different value than the one requested\n");
            log_position(&(__position__config.dvalue));
        }
        else
        {
            if (__position__config.request_time)
                DEBUG_PRINT("GAP has set position config after %ld ms to\n", now - __position__config.request_time);
            else
                DEBUG_PRINT("GAP has set position config to\n");

            log_position(&(__position__config.value));
        }
    }
#endif // VERBOSE_RX
    __position__config.dvalue = __position__config.value;
    __position__config.request_time = 0;
}

void save_position_values(position_t *savehere)
{
    savehere->xc = __position__config.value.xc;
    savehere->yc = __position__config.value.yc;
    savehere->height = __position__config.value.height;
    savehere->width = __position__config.value.width;
}


// --- config detection

void log_detection(detection_t *value)
{
    DEBUG_PRINT("xt=%d, yt=%d, height=%d, width=%d, probability=%.3f\n", value->xt, value->yt, value->height, value->width, (double)value->prob);
}

static struct
{
    uint32_t request_time;
    detection_t value, dvalue;
    const char *header;
} __detection__config = {.request_time = 0, .header = "!DET"};

void __detection_cb(void *buffer)
{
    detection_t *value = (detection_t *)buffer;
    __detection__config.value = *value;

#ifdef VERBOSE_RX
    if (verbose_rx)
    {
        uint32_t now = T2M(xTaskGetTickCount());

        if (__detection__config.request_time && memcmp(&(__detection__config.value), &(__detection__config.dvalue), sizeof(detection_t)))
        {
            DEBUG_PRINT("[WARNING] GAP has set detection config after %ld ms\n", now - __detection__config.request_time);
            log_detection(&(__detection__config.value));
            DEBUG_PRINT("to a different value than the one requested\n");
            log_detection(&(__detection__config.dvalue));
        }
        else
        {
            if (__detection__config.request_time)
                DEBUG_PRINT("GAP has set detection config after %ld ms to\n", now - __detection__config.request_time);
            else
                DEBUG_PRINT("GAP has set detection config to\n");

            log_detection(&(__detection__config.value));
        }
    }
#endif // VERBOSE_RX
    __detection__config.dvalue = __detection__config.value;
    __detection__config.request_time = 0;
}

void save_detection_values(detection_t *savehere)
{
    savehere->xt = __detection__config.value.xt;
    savehere->yt = __detection__config.value.yt;
    savehere->height = __detection__config.value.height;
    savehere->width = __detection__config.value.width;
    savehere->prob = __detection__config.value.prob;
}
