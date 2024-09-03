#include "WS2812.h"
#include "WS2812.pio.h"
#include "hardware/dma.h"
#include <stdio.h>
#include <stdlib.h>

static color_t convert_data(ws2812_t* l, rgbw_t rgbw);
static void wait_for_dma(ws2812_t* l);

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
	l->dma_chan = dma_claim_unused_channel(true);
	dma_channel_config c = dma_channel_get_default_config(l->dma_chan);
	channel_config_set_transfer_data_size(&c, DMA_SIZE_32);
	channel_config_set_read_increment(&c, true);
	channel_config_set_dreq(&c, DREQ_PIO0_TX0);
	// channel_config_set_bswap(&c, true); // Swap so the data comes out in the right order since
	// the
	// 									// endianess messes up the order
	dma_channel_configure(l->dma_chan, &c,
						  &pio0_hw->txf[0], // Write address (only need to set this once)
						  NULL, // Don't provide a read address yet
						  l->length, // Write the same value many times, then halt and interrupt
						  false // Don't start yet
	);
}

color_t ws2812_RGBW(ws2812_t* l, const rgbw_t raw_color)
{
	return convert_data(l, raw_color);
}

void ws2812_setPixelColor(ws2812_t* l, uint index, color_t color)
{
	wait_for_dma(l);
	if(index < l->length)
	{
		l->data[index] = color;
	}
}

void ws2812_fill(ws2812_t* l, color_t color, uint first, uint count)
{
	uint last = (first + count);
	wait_for_dma(l);

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
	// Wait if there is a current transfer going
	wait_for_dma(l);
	dma_channel_set_read_addr(l->dma_chan, l->data, true);
}

static color_t convert_data(ws2812_t* l, rgbw_t rgbw)
{
	uint32_t result = 0;

	for(uint b = 0; b < 4; b++)
	{
		switch(l->bytes[b])
		{
			case RED:
				result |= rgbw.red;
				break;
			case GREEN:
				result |= rgbw.green;
				break;
			case BLUE:
				result |= rgbw.blue;
				break;
			case WHITE:
				result |= rgbw.white;
				break;
		}
		result <<= 8;
	}
	return result;
}

static void wait_for_dma(ws2812_t* l)
{
	while(dma_channel_is_busy(l->dma_chan))
		;
}