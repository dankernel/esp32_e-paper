/**
*  @filename   :   main.cpp
*  @brief      :   1.54inch e-paper display demo
*  @author     :   Yehui from Waveshare
*  @modified   :   dankernel
*
*  Copyright (C) Waveshare     September 9 2017
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documnetation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to  whom the Software is
* furished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "epd1in54.h"
#include "epdpaint.h"
#include "imagedata.h"

#define COLORED      0
#define UNCOLORED    1

extern "C" void app_main()
{
	time_t now;
	struct tm* timenow;

	Epd epd;
	if (epd.IfInit() != 0) {
		ESP_LOGI("DKDK", "init failed");
		return;
	}

	if (epd.Init(lut_full_update) != 0) {
		ESP_LOGI("DKDK", "init failed");
		return;
	}

	unsigned char* frame_buffer = (unsigned char*)malloc(epd.width / 8 * epd.height);

	Paint paint(frame_buffer, epd.width, epd.height);

	/* For simplicity, the arguments are explicit numerical coordinates */
	/* Write strings to the buffer */
	paint.Clear(UNCOLORED);
	paint.DrawFilledRectangle(0, 0, 200, 50, COLORED);
	paint.DrawStringAt(25, 10, "Jun Hyung Park", &Font16, UNCOLORED);
	paint.DrawStringAt(25, 30, "I love jisu", &Font16, UNCOLORED);

	paint.DrawFilledRectangle(0, 100, 200, 150, COLORED);
	paint.DrawStringAt(25, 110, "123 456 789", &Font16, UNCOLORED);
	paint.DrawStringAt(25, 130, "ABC DEF XYZ", &Font16, UNCOLORED);

	/* Draw something to the frame_buffer */
	epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
	epd.DisplayFrame();
	epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
	epd.DisplayFrame();

	epd.DelayMs(2000);
	ESP_LOGI("DKDK", "Print Rect");

	if (epd.Init(lut_partial_update) != 0) {
		printf("e-Paper init failed\n");
		return;
	}

	char time_string[] = {'0', '0', ':', '0', '0', '\0'};
	while (1) {
		time(&now);
		timenow = localtime(&now);
		time_string[0] = timenow->tm_min / 10 + '0';
		time_string[1] = timenow->tm_min % 10 + '0';
		time_string[3] = timenow->tm_sec / 10 + '0';
		time_string[4] = timenow->tm_sec % 10 + '0';

		paint.SetWidth(100);
		paint.SetHeight(40);
		paint.Clear(UNCOLORED);
		paint.DrawStringAt(0, 10, time_string, &Font24, COLORED);

		epd.SetFrameMemory(paint.GetImage(), 50, 52, paint.GetWidth(), paint.GetHeight());
		epd.DisplayFrame();
		epd.SetFrameMemory(paint.GetImage(), 50, 52, paint.GetWidth(), paint.GetHeight());
		epd.DisplayFrame();

		epd.DelayMs(500);
		ESP_LOGI("DKDK", "Print!");
	}
	// Start App
	printf("Hello world!\n");


	// Restart App
	printf("Restarting now.\n");
	fflush(stdout);
	esp_restart();

	return;
}
