#include <stdint.h>

#include "common/pimoroni_common.hpp"
#include "headers/CustomBadger.hpp"
#include "headers/Menu.hpp"
#include "headers/MenuItems.hpp"
#include "image.hpp"
#include "pico/platform.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/time.h"

using namespace pimoroni;
MyloBadger badger;

Menu* currentMenu;

int main() {
	stdio_init_all();

	initializeMenu();
	currentMenu = mainMenu;

	badger.init();
	badger.protoInit();
	badger.waitForProtoLink();

	while (true) {
		badger.led(63);
		currentMenu->display();
		badger.wait_for_press();

		badger.led(255);
		if (badger.pressed(badger.UP)) {
			currentMenu->up();
		}

		if (badger.pressed(badger.DOWN)) {
			currentMenu->down();
		}

		if (badger.pressed(badger.A)) {
			Menu* nextMenu = currentMenu->click();

			if (nextMenu == NULL) {
			} else {
				currentMenu = nextMenu;
			}
		}

		if (badger.pressed(badger.B)) {
			Menu* nextMenu = currentMenu->back();

			if (nextMenu == NULL) {
			} else {
				currentMenu = nextMenu;
			}
		}

		if (badger.pressed(badger.C)) {
			currentMenu->action();
		}
	}

	badger.halt();
}