#include "WS2812.h"
#include "WS2812.pio.h"
#include <stdio.h>
#include <stdlib.h>

static color_t convert_data(ws2812_t* l, rgbw_t rgbw);

void ws2812_initialize(ws2812_t* l, uint pin, uint length, PIO pio, uint sm, data_format_t format)
{
	switch(format)
	{
		case FORMAT_RGB:
			l->bytes[0] = NONE;
			l->bytes[1] = RED;
			l->bytes[2] = GREEN;
			l->bytes[3] = BLUE;
			break;
		case FORMAT_GRB:
			l->bytes[0] = NONE;
			l->bytes[1] = GREEN;
			l->bytes[2] = RED;
			l->bytes[3] = BLUE;
			break;
		case FORMAT_WRGB:
			l->bytes[0] = WHITE;
			l->bytes[1] = RED;
			l->bytes[2] = GREEN;
			l->bytes[3] = BLUE;
			break;
	}

	l->pin = pin;
	l->length = length;
	l->pio = pio;
	l->sm = sm;
	l->data = (color_t*)malloc(length * sizeof(color_t));
	uint offset = pio_add_program(pio, &ws2812_program);
	uint bits = (l->bytes[0] == NONE ? 24 : 32);
	ws2812_program_init(pio, sm, offset, pin, 800000, bits);
}

color_t ws2812_RGBW(ws2812_t* l, const rgbw_t raw_color)
{
	return convert_data(l, raw_color);
}

void ws2812_setPixelColor(ws2812_t* l, uint index, color_t color)
{
	if(index < l->length)
	{
		l->data[index] = color;
	}
}

void ws2812_fill(ws2812_t* l, color_t color, uint first, uint count)
{
	uint last = (first + count);
	if(last > l->length)
	{
		last = l->length;
	}
	for(uint i = first; i < last; i++)
	{
		l->data[i] = color;
	}
}

void ws2812_show(ws2812_t* l)
{
	for(uint i = 0; i < l->length; i++)
	{
		pio_sm_put_blocking(l->pio, l->sm, l->data[i]);
	}
}

static color_t convert_data(ws2812_t* l, rgbw_t rgbw)
{
	uint32_t result = 0;

	for(uint b = 0; b < 4; b++)
	{
		switch(l->bytes[b])
		{
			case RED:
				result <<= 8;
				result |= rgbw.red;
				break;
			case GREEN:
				result <<= 8;
				result |= rgbw.green;
				break;
			case BLUE:
				result <<= 8;
				result |= rgbw.blue;
				break;
			case WHITE:
				result <<= 8;
				result |= rgbw.white;
				break;
		}
	}
	return result;
}
