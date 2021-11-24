#ifndef __AIDECK_PROTOCOL_H__
#define __AIDECK_PROTOCOL_H__

#include <stdint.h>
#include <stdbool.h>
#include "debug.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "stm32fxxx.h"
#include "config.h"
#include "console.h"
#include "uart1.h"
#include "deck.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "log.h"
#include "param.h"
#include "system.h"
#include "uart1.h"
#include "uart2.h"

#include "task.h"


#define VERBOSE_RX
#define VERBOSE_TX

#define HEADER_LENGTH 4
#define BUFFER_LENGTH 13
#define INPUT_NUMBER 4


typedef struct {
  const char *header;
  uint8_t size;
  void (*callback)(void *);
  bool valid;
} input_t;


typedef struct {
  uint16_t marginTop;
  uint16_t marginRight;
  uint16_t marginBottom;
  uint16_t marginLeft;
  uint8_t format;
  uint8_t step;
  uint8_t target_value;
  uint8_t ae;
  uint8_t fps;
} __attribute__((packed)) camera_t;

typedef struct {
  uint8_t on;
  uint8_t format;
  uint8_t transport;
} __attribute__((packed)) stream_t;


typedef struct {
  uint16_t xc;
  uint16_t yc;
  uint16_t height;
  uint16_t width;
} __attribute__((packed)) position_t;

typedef struct {
  uint16_t xt;
  uint16_t yt;
  uint16_t width;
  uint16_t height;
  float_t prob;
} __attribute__((packed)) detection_t;


void __camera_cb(void *buffer);
void __stream_cb(void *buffer);
void __position_cb(void *buffer);
void __detection_cb(void *buffer);

void save_stream_values(stream_t *savehere);
void save_position_values(position_t *save);
void save_detection_values(detection_t *save);

#endif //__AIDECK_PROTOCOL_H__
