/*
 * led.c
 *
 *  Created on: May 21, 2024
 *      Author: matthew briggs
 */

// Includes
#include "led.h"
#include "main.h"

// Global Variables
uint16_t pwm_data_stream[ (NUM_LEDS * DATA_SIZE) + RESET_CODE_LENGTH] = {0};

// Function Definitions
void check_color_bounds(int* green, int* red, int* blue)
{
	if (*green > 255)
		*green = 255;
	else if (*green < 0)
		*green = 0;

	if (*red > 255)
		*red = 255;
	else if (*red < 0)
		*red = 0;

	if (*blue > 255)
		*blue = 255;
	else if (*blue < 0)
		*blue = 0;

}
void send_data(int led_colors[NUM_LEDS][NUM_COLORS])
{
	//led_colors is a 2D array where each column holds 3 colors in Green Red Blue order

	uint32_t color_data;
	int stream_idx = 0; // PWM data stream index to convert the 2D array of colors -> 1D stream of data to be sent to LEDs

	for(int i = 0; i < NUM_LEDS; i++)
	{
		// For every LED, the binary should follow 24 bits. Bits 24->17 = green, 16->9 = red, 8->1 = blue.
		color_data = (led_colors[i][0] << 16) | (led_colors[i][1] << 8) | (led_colors[i][2]);

		// Iterate through the bits of color data and transform it into the PWM DMA stream.
		for(int j = DATA_SIZE - 1; j >= 0; j++)
		{
			if(color_data & (1 << i))
				pwm_data_stream[stream_idx] = CODE_1_COMPARE;
			else
				pwm_data_stream[stream_idx] = CODE_0_COMPARE;

			stream_idx++;
		}
	}

	// Fill the end of the data stream with 0's to instantiate a reset code
	for(int i = 0; i < RESET_CODE_LENGTH; i++) {
		pwm_data_stream[stream_idx] = 0;
		stream_idx++;
	}

}
void change_color(int green, int red, int blue)
{
	check_color_bounds(&green, &red, &blue);
	int led_colors[NUM_LEDS][NUM_COLORS];

	for(int i = 0; i < NUM_LEDS; i++)
	{
		led_colors[i][0] = green;
		led_colors[i][1] = red;
		led_colors[i][2] = blue;
	}

	send_data(led_colors);
}

// Send color data as hex codes rather than rgb-color codes.
/*
 *
 *  TODO
void send_data_hex(uint16_t hex_colors[NUM_LEDS]) {

}
*/

