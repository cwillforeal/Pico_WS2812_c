#include <stdio.h>
#include "WS2812.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"

#define LED_PIN 0
#define LED_LENGTH 6

int main()
{
	ws2812_t led;
	rgbw_t color;

	ws2812_initialize(&led, LED_PIN, LED_LENGTH, pio0, 0, FORMAT_RGB);

	color.blue = 0;
	color.green = 0;
	color.red = 125;

	ws2812_fill(&led, ws2812_RGBW(&led, color), 0, LED_LENGTH);
	ws2812_show(&led);

	while(1)
		;
	return 0;
}
