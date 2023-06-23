#include "../headers/CustomBadger.hpp"

#include "hardware/uart.h"
#include "pico/time.h"

void MyloBadger::protoInit() {
	this->update_speed(2);
	this->font("bitmap8");
	this->pen(15);
	this->clear();
	this->pen(0);
	this->text("Establishing Link...", 0, 0, 3);
	this->text("Press A to continue without a link", 0, DISP_HEIGHT - 16, 1);
	this->text("HiFi Protogen OS V1.1", 0, DISP_HEIGHT - 8, 1);
	uart_init(uart0, 115200);
	gpio_set_function(0, GPIO_FUNC_UART);
	gpio_set_function(1, GPIO_FUNC_UART);

	uart_set_hw_flow(uart0, false, false);
	uart_set_format(uart0, 8, 1, UART_PARITY_NONE);

	this->update(true);
	sleep_ms(1000);
}

void MyloBadger::waitForProtoLink() {
	bool skipped = false;

	while (true) {
		uint8_t inChar;
		if (uart_is_readable_within_us(uart0, 5000)) {
			uart_read_blocking(uart0, &inChar, 1);
			if (inChar == 'B') {
				uart_putc_raw(uart0, 'B');
				uart_read_blocking(uart0, &inChar, 1);
				if (inChar == 'G') {
					// we good
					break;
				}
			}
		} else {
			this->update_button_states();
			if (this->pressed(this->A)) {
				skipped = true;
				break;
			}
		}
	}

	this->pen(15);
	this->clear();
	this->pen(0);
	if (skipped) {
		this->text("Skipped!", 0, 0, 3);
	} else {
		this->text("Link established!", 0, 0, 3);
	}
	this->text("HiFi Protogen OS V1.1", 0, DISP_HEIGHT - 8, 1);
	this->update(true);
	sleep_ms(1000);
}

void MyloBadger::drawMyloImage(MyloImage image) {
	this->led(255);
	uint8_t* currentBitPlane = new uint8_t[DISP_PIXELS / 8];
	for (uint_fast16_t ii = 0; ii < DISP_PIXELS; ii++) {
		currentBitPlane[ii] = 0;
	}

	uint16_t bitplaneIndex = 0;

	uint8_t currentByte = 0;
	uint8_t currentByteOffset = 0;

	uint8_t valuesOffset = 0;

	for (int_fast8_t ii = 3; ii > 0; ii--) {
		this->led(255);
		bitplaneIndex = 0;
		currentByte = 0;
		currentByteOffset = 0;

		valuesOffset = 8;

		for (uint_fast16_t jj = 0; jj < image.countLength; jj++) {
			uint8_t currentValuesByte = image.values[jj / 4];
			bool currentValue = ((currentValuesByte >> (valuesOffset - 2)) & 0x03) == ii ? true : false;

			for (uint_fast8_t kk = 0; kk < image.counts[jj]; kk++) {
				currentByte = currentByte << 1;
				currentByte = currentByte | currentValue;
				currentByteOffset++;

				if (currentByteOffset >= 8) {
					currentBitPlane[bitplaneIndex] = currentBitPlane[bitplaneIndex] | currentByte;
					bitplaneIndex++;
					currentByteOffset = 0;
				}
			}
			valuesOffset = valuesOffset - 2;

			if (valuesOffset == 0) {
				valuesOffset = 8;
			}
		}

		if (ii == 3)
			this->update_speed(0);
		else if (ii == 2)
			this->update_speed(3);

		this->led(170);
		this->image(currentBitPlane);
		this->led(85);
		this->update(true);
	}
	delete currentBitPlane;
}