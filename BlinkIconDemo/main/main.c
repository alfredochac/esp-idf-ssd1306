#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "ssd1306.h"

/*
 You have to set this config value with menuconfig
 CONFIG_INTERFACE

 for i2c
 CONFIG_MODEL
 CONFIG_SDA_GPIO
 CONFIG_SCL_GPIO
 CONFIG_RESET_GPIO

 for SPI
 CONFIG_CS_GPIO
 CONFIG_DC_GPIO
 CONFIG_RESET_GPIO
*/

#define TAG "SSD1306"

// https://thenounproject.com/icon/receive-data-3177814/
uint8_t receive_white[] = {
// 'receive-data-white', 64x64px
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xfc, 0x00, 0x3f, 0xe0, 0x1f, 0xff, 0xff, 0xff, 0xf8, 0xff, 0x9f, 0xe7, 0x9f, 0xff, 0xff, 
0xff, 0xf9, 0xff, 0x9f, 0xef, 0x9f, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xcf, 0xef, 0x9f, 0xff, 0xff, 
0xff, 0xf3, 0xff, 0xe7, 0xcf, 0x9f, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x7f, 
0xff, 0xf0, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x7f, 0xff, 0xf3, 0xff, 0xff, 0xef, 0x9f, 0xfe, 0x7f, 
0xff, 0xf3, 0xff, 0xff, 0xef, 0x9f, 0xfe, 0x7f, 0xff, 0xf3, 0xff, 0xff, 0xef, 0x9f, 0xfe, 0x7f, 
0xff, 0xf3, 0xff, 0xff, 0xef, 0x9f, 0xfe, 0x7f, 0xff, 0xf3, 0xff, 0xff, 0xcf, 0x9f, 0xfe, 0x7f, 
0xfe, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x3e, 0x7f, 0xfe, 0x7f, 0xff, 0xff, 0xcf, 0x8f, 0x3e, 0x7f, 
0xfe, 0x7f, 0xff, 0xff, 0xef, 0x9f, 0x3e, 0x7f, 0xfe, 0x7f, 0xff, 0xfc, 0x0f, 0x81, 0x3e, 0x7f, 
0xfe, 0x7f, 0xff, 0xfe, 0x0f, 0x83, 0x3e, 0x7f, 0xfe, 0x70, 0xc0, 0x0f, 0x1f, 0xe7, 0x3e, 0x7f, 
0xfe, 0x79, 0xc0, 0x1f, 0x8f, 0xcf, 0x3e, 0x7f, 0xfe, 0x7f, 0xff, 0xff, 0xc7, 0x9f, 0x3e, 0x7f, 
0xfe, 0x7f, 0xff, 0xff, 0xe3, 0x3f, 0x3e, 0x7f, 0xfe, 0x7f, 0xff, 0xff, 0xf0, 0x7f, 0x3e, 0x7f, 
0xfe, 0x70, 0xc0, 0x00, 0x78, 0xff, 0x3e, 0x7f, 0xfe, 0x79, 0xc0, 0x00, 0xfd, 0xff, 0x3e, 0x7f, 
0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x3e, 0x7f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x3e, 0x7f, 
0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x3e, 0x7f, 0xfe, 0x78, 0xc0, 0x00, 0x00, 0x07, 0x3e, 0x7f, 
0xfe, 0x78, 0xc0, 0x00, 0x00, 0x07, 0x3e, 0x7f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x3e, 0x7f, 
0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x3e, 0x7f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x3e, 0x7f, 
0xfe, 0x79, 0xc0, 0x00, 0x00, 0x0f, 0x3e, 0x7f, 0xfe, 0x70, 0xc0, 0x00, 0x00, 0x07, 0x3e, 0x7f, 
0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x3e, 0x7f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x3e, 0x7f, 
0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x9e, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 
0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

// https://thenounproject.com/icon/receive-data-3177850/
uint8_t receive_black[] = {
// 'receive-data-black', 64x64px
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xf8, 0x7f, 0xff, 0xff, 
0xff, 0xfe, 0x00, 0x7f, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x3f, 0xf0, 0x7f, 0xff, 0xff, 
0xff, 0xfc, 0x00, 0x1f, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x10, 0x60, 0x01, 0xff, 
0xff, 0xfc, 0x00, 0x00, 0x10, 0x60, 0x01, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x10, 0x60, 0x01, 0xff, 
0xff, 0xfc, 0x00, 0x00, 0x10, 0x60, 0x01, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x30, 0x60, 0x01, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xf0, 0x7f, 0xc1, 0xff, 0xff, 0x80, 0x00, 0x00, 0x30, 0x70, 0xc1, 0xff, 
0xff, 0x80, 0x00, 0x00, 0x10, 0x60, 0xc1, 0xff, 0xff, 0x80, 0x00, 0x03, 0xf0, 0x7e, 0xc1, 0xff, 
0xff, 0x80, 0x00, 0x01, 0xf0, 0x3c, 0xc1, 0xff, 0xff, 0x8f, 0x3f, 0xf0, 0xe0, 0x18, 0xc1, 0xff, 
0xff, 0x86, 0x3f, 0xe0, 0x70, 0x30, 0xc1, 0xff, 0xff, 0x80, 0x00, 0x00, 0x38, 0x60, 0xc1, 0xff, 
0xff, 0x80, 0x00, 0x00, 0x1c, 0xc0, 0xc1, 0xff, 0xff, 0x80, 0x00, 0x00, 0x0f, 0x80, 0xc1, 0xff, 
0xff, 0x8f, 0x3f, 0xff, 0x87, 0x00, 0xc1, 0xff, 0xff, 0x86, 0x3f, 0xff, 0x02, 0x00, 0xc1, 0xff, 
0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xc1, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xc1, 0xff, 
0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xc1, 0xff, 0xff, 0x86, 0x3f, 0xff, 0xff, 0xf8, 0xc1, 0xff, 
0xff, 0x86, 0x3f, 0xff, 0xff, 0xf8, 0xc1, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xc1, 0xff, 
0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xc1, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xc1, 0xff, 
0xff, 0x86, 0x3f, 0xff, 0xff, 0xf0, 0xc1, 0xff, 0xff, 0x8f, 0x3f, 0xff, 0xff, 0xf8, 0xc1, 0xff, 
0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xc1, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xc1, 0xff, 
0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x61, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

// https://thenounproject.com/icon/data-send-3753756/
uint8_t send_white[] = {
// 'data-send-white', 64x64px
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0x80, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 
0xff, 0x1f, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 
0xff, 0x1f, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0x80, 0x00, 0x00, 0x1f, 0xff, 
0xff, 0x1f, 0xff, 0x80, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x1f, 0xff, 0xe0, 0x00, 0x00, 0x0f, 0xff, 
0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 
0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 
0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 
0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 
0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 
0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 
0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 
0xff, 0x1f, 0xff, 0xff, 0xff, 0xc7, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 
0xff, 0x1f, 0xff, 0xff, 0xff, 0xc1, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xc0, 0x7f, 0xff, 
0xff, 0x1f, 0xff, 0xff, 0xff, 0x8c, 0x3f, 0xff, 0xff, 0x1f, 0xff, 0xff, 0x00, 0x0e, 0x1f, 0xff, 
0xff, 0x1f, 0xff, 0xff, 0x00, 0x0f, 0x0f, 0xff, 0xff, 0x1f, 0xff, 0xff, 0x1f, 0xff, 0x87, 0xff, 
0xff, 0x1f, 0xff, 0xff, 0x1f, 0xff, 0xc1, 0xff, 0xff, 0x1f, 0xff, 0xff, 0x1f, 0xff, 0xf0, 0xff, 
0xff, 0x1f, 0xff, 0xff, 0x1f, 0xff, 0xf0, 0xff, 0xff, 0x1f, 0xff, 0xff, 0x1f, 0xff, 0xe1, 0xff, 
0xff, 0x1f, 0xff, 0xff, 0x1f, 0xff, 0xc3, 0xff, 0xff, 0x00, 0x00, 0x1f, 0x00, 0x0f, 0x07, 0xff, 
0xff, 0x80, 0x00, 0x0f, 0x00, 0x0e, 0x1f, 0xff, 0xff, 0xc0, 0x00, 0x1f, 0x80, 0x0c, 0x3f, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xc1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

// https://thenounproject.com/icon/data-send-3753855/
uint8_t send_black[] = {
// 'data-send-black', 64x64px
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xc0, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 
0xff, 0x80, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 
0xff, 0x80, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 
0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 
0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 
0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 
0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 
0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 
0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 
0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 
0xff, 0x80, 0x00, 0x00, 0x00, 0x30, 0x0f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x78, 0x0f, 0xff, 
0xff, 0x80, 0x00, 0x00, 0x00, 0x7c, 0x0f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x7e, 0x0f, 0xff, 
0xff, 0x80, 0x00, 0x00, 0x00, 0x77, 0x0f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x73, 0xcf, 0xff, 
0xff, 0x80, 0x00, 0x00, 0x00, 0x71, 0xef, 0xff, 0xff, 0x80, 0x00, 0x03, 0xff, 0xf0, 0x7f, 0xff, 
0xff, 0x80, 0x00, 0x07, 0xff, 0xe0, 0x3f, 0xff, 0xff, 0x80, 0x00, 0x07, 0x00, 0x00, 0x1f, 0xff, 
0xff, 0x80, 0x00, 0x07, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x80, 0x00, 0x07, 0x00, 0x00, 0x07, 0xff, 
0xff, 0x80, 0x00, 0x07, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x07, 0x00, 0x00, 0x01, 0xff, 
0xff, 0x80, 0x00, 0x07, 0x00, 0x00, 0x03, 0xff, 0xff, 0x80, 0x00, 0x07, 0x00, 0x00, 0x07, 0xff, 
0xff, 0xc0, 0x00, 0x07, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x3f, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

void app_main(void)
{
	SSD1306_t dev;

#if CONFIG_I2C_INTERFACE
	ESP_LOGI(TAG, "INTERFACE is i2c");
	ESP_LOGI(TAG, "CONFIG_SDA_GPIO=%d",CONFIG_SDA_GPIO);
	ESP_LOGI(TAG, "CONFIG_SCL_GPIO=%d",CONFIG_SCL_GPIO);
	ESP_LOGI(TAG, "CONFIG_RESET_GPIO=%d",CONFIG_RESET_GPIO);
	i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
#endif // CONFIG_I2C_INTERFACE

#if CONFIG_SPI_INTERFACE
	ESP_LOGI(TAG, "INTERFACE is SPI");
	ESP_LOGI(TAG, "CONFIG_MOSI_GPIO=%d",CONFIG_MOSI_GPIO);
	ESP_LOGI(TAG, "CONFIG_SCLK_GPIO=%d",CONFIG_SCLK_GPIO);
	ESP_LOGI(TAG, "CONFIG_CS_GPIO=%d",CONFIG_CS_GPIO);
	ESP_LOGI(TAG, "CONFIG_DC_GPIO=%d",CONFIG_DC_GPIO);
	ESP_LOGI(TAG, "CONFIG_RESET_GPIO=%d",CONFIG_RESET_GPIO);
	spi_master_init(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO);
#endif // CONFIG_SPI_INTERFACE

#if CONFIG_FLIP
	dev._flip = true;
	ESP_LOGW(TAG, "Flip upside down");
#endif

#if CONFIG_SSD1306_128x64
	ESP_LOGI(TAG, "Panel is 128x64");
	ssd1306_init(&dev, 128, 64);
#endif // CONFIG_SSD1306_128x64
#if CONFIG_SSD1306_128x32
	ESP_LOGE(TAG, "Panel is 128x32. This demo cannot be run.");
	while(1) { vTaskDelay(1); }
#endif // CONFIG_SSD1306_128x32

	ssd1306_contrast(&dev, 0xff);
	ssd1306_clear_screen(&dev, true);
	while(1) {
		ssd1306_bitmaps(&dev, 31, 0, receive_white, 64, 64, false);
		vTaskDelay(50);
		ssd1306_bitmaps(&dev, 31, 0, receive_black, 64, 64, false);
		vTaskDelay(50);
		ssd1306_bitmaps(&dev, 31, 0, send_white, 64, 64, false);
		vTaskDelay(50);
		ssd1306_bitmaps(&dev, 31, 0, send_black, 64, 64, false);
		vTaskDelay(50);
	}
}
