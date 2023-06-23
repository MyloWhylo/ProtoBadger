#include "../headers/SettingsMenu.hpp"

#include "string.h"
#include "hardware/uart.h"

using namespace pimoroni;
extern MyloBadger badger;

IntegerSetting::IntegerSetting(const char* menuName, char cmdPrefix, uint8_t initialValue, Menu* parent) : Menu(menuName, 0, nullptr, parent) {
	this->val = initialValue;
	this->oldVal = initialValue;
	this->prefix = cmdPrefix;
}

void IntegerSetting::drawTitleBar() {
	badger.text(this->name, 0, 0, 3.0f);
}

void IntegerSetting::drawNavBar() {
	badger.pen(0);
	badger.rectangle(0, DISP_HEIGHT - 10, DISP_WIDTH - 1, DISP_HEIGHT - 1);
	badger.pen(15);

	int_fast16_t setPos = 40 - (badger.measure_text("Set") / 2);
	badger.text("Set", setPos, DISP_HEIGHT - 9);

	if (this->oldVal != this->val) {
		int_fast16_t backPos = (DISP_WIDTH / 2) - (badger.measure_text("Cancel") / 2);
      badger.text("Cancel", backPos, DISP_HEIGHT - 9);
	} else {
		int_fast16_t backPos = (DISP_WIDTH / 2) - (badger.measure_text("Back") / 2);
		badger.text("Back", backPos, DISP_HEIGHT - 9);
	}

	badger.pen(0);
}

void IntegerSetting::drawMenuOptions() {
	char dispStr[15];

	int len = snprintf(dispStr, sizeof(char) * 14, "Old Value: %u", this->oldVal);
	dispStr[len] = '\0';
	badger.text(dispStr, 0, 25, 2);

	len = snprintf(dispStr, sizeof(char) * 14, "New Value: %u", this->val);
	dispStr[len] = '\0';
	badger.text(dispStr, 0, 25 + (8 * 2), 2);

	if (this->val == this->oldVal) {
		badger.text("Active on Proto", 0, 25 + ((8 * 2) * 2), 2);
	}
}

void IntegerSetting::up() {
	if (this->val != 255) this->val++;
}

void IntegerSetting::down() {
	if (this->val != 0) this->val--;
}

Menu* IntegerSetting::click() {
	this->oldVal = this->val;
	uart_putc_raw(uart0, this->prefix);
	uart_putc_raw(uart0, this->oldVal);
	uart_putc_raw(uart0, 0);
	return this;
}

Menu* IntegerSetting::back() {
	badger.update_speed(2);
   
   if (this->oldVal != this->val) {
      this->val = this->oldVal;
   }

	return this->parentMenu;
}

// void IntegerSetting::action() {
// 	badger.text("Pushed to Proto!", 0, 25 + ((8 * 2) * 2), 2);
// }