#ifndef WS2812C_H
#define WS2812C_H

#include "pico/types.h"
#include "hardware/pio.h"
#include <stdint.h>

typedef enum
{
	NONE = 0,
	RED = 1,
	GREEN = 2,
	BLUE = 3,
	WHITE = 4
} data_byte_t;

typedef enum
{
	FORMAT_RGB = 0,
	FORMAT_GRB = 1,
	FORMAT_WRGB = 2
} data_format_t;

typedef struct
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t white;
} rgbw_t;

typedef uint32_t color_t;

typedef struct
{
	uint pin;
	uint length;
	PIO pio;
	uint sm;
	data_byte_t bytes[4];
	color_t* data;
	int dma_chan;
} ws2812_t;

void ws2812_initialize(ws2812_t* l, uint pin, uint length, PIO pio, uint sm, data_format_t format);
color_t ws2812_RGBW(ws2812_t* l, rgbw_t rgbw);
void ws2812_fill(ws2812_t* l, color_t color, uint first, uint count);
void ws2812_show(ws2812_t* l);
void ws2812_setPixelColor(ws2812_t* l, uint index, color_t color);

#endif