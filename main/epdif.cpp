/**
 *  @filename   :   epdif.cpp
 *  @brief      :   Implements EPD interface functions
 *                  Users have to implement all the functions in epdif.cpp
 *  @author     :   Yehui from Waveshare
 *  @modified   :   dankernel
 *
 *  Copyright (C) Waveshare     August 10 2017
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

#include "epdif.h"
// TODO
// #include <wiringPi.h>
// #include <wiringPiSPI.h>
// Add
#include "driver/gpio.h"

#define SPI_MOSI_PIN 23
#define SPI_CLK_PIN 18
#define SPI_CS_PIN 5
#define DC_PIN 2

EpdIf::EpdIf() {
};
EpdIf::~EpdIf() {
};

void EpdIf::DigitalWrite(int pin, int value) {
    // digitalWrite(pin, value);
    gpio_set_level((gpio_num_t)pin, value);
}

int EpdIf::DigitalRead(int pin) {
    return gpio_get_level((gpio_num_t)pin);
}

void EpdIf::DelayMs(unsigned int delaytime) {
  vTaskDelay(delaytime / portTICK_RATE_MS);
}

void EpdIf::SpiTransfer(unsigned char data) {
	// wiringPiSPIDataRW(0, &data, 1);
	this->trans_desc.addr = 0;
	this->trans_desc.cmd = 0;
	this->trans_desc.flags = 0;
	this->trans_desc.length = 1 * 8;
	this->trans_desc.rxlength = 0;
	this->trans_desc.tx_buffer = &data;
	this->trans_desc.rx_buffer = &data;

	ESP_ERROR_CHECK(spi_device_transmit(this->handle, &this->trans_desc));
}

int EpdIf::IfInit(void) {

	// Bus config
	this->bus_config.sclk_io_num = SPI_CLK_PIN; // CLK
	this->bus_config.mosi_io_num = SPI_MOSI_PIN; // MOSI
	this->bus_config.miso_io_num = -1; // MISO
	this->bus_config.quadwp_io_num = -1; // Not used
	this->bus_config.quadhd_io_num = -1; // Not used
	ESP_LOGI("DKDK", "... Initializing bus.");
	ESP_ERROR_CHECK(spi_bus_initialize(HSPI_HOST, &this->bus_config, 0));

	// SPI config
	this->dev_config.address_bits = 0;
	this->dev_config.command_bits = 0;
	this->dev_config.dummy_bits = 0;
	this->dev_config.mode = 0;
	this->dev_config.duty_cycle_pos = 0;
	this->dev_config.cs_ena_posttrans = 0;
	this->dev_config.cs_ena_pretrans = 0;
	this->dev_config.clock_speed_hz = 2000000;
	this->dev_config.spics_io_num = SPI_CS_PIN;
	this->dev_config.flags = SPI_DEVICE_NO_DUMMY;
	this->dev_config.queue_size = 1;
	this->dev_config.pre_cb = NULL;
	this->dev_config.post_cb = NULL;
	ESP_LOGI("DKDK", "... Adding device bus.");
	ESP_ERROR_CHECK(spi_bus_add_device(HSPI_HOST, &this->dev_config, &this->handle));
	ESP_LOGI("DKDK", "OK adding device");

	gpio_set_direction((gpio_num_t)DC_PIN, GPIO_MODE_OUTPUT);
	ESP_LOGI("DKDK", "OK ifinit");
	return 0;
}

