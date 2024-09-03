#include <stdio.h>
#include "WS2812.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include "pico/time.h"

#define LED_PIN 0
#define LED_LENGTH 12

int main()
{
	ws2812_t led;
	rgbw_t color;

	ws2812_initialize(&led, LED_PIN, LED_LENGTH, pio0, 0, FORMAT_GRB);

	color.blue = 0x00;
	color.green = 0x00;
	color.red = 0x0;

	while(1)
	{
		ws2812_fill(&led, ws2812_RGBW(&led, color), 0, LED_LENGTH);
		ws2812_show(&led);
		sleep_ms(10);
	}
	return 0;
}
