#include "../headers/ClearMenuItem.hpp"

#include "../headers/CustomBadger.hpp"

using namespace pimoroni;
extern MyloBadger badger;

ClearMenuItem::ClearMenuItem(const char* name, Menu* parentMenu) : MenuItem(name, parentMenu){};

Menu* ClearMenuItem::handleInteraction() {
	badger.update_speed(0);
	badger.pen(15);
	badger.clear();
	badger.update(true);
	badger.led(0);
	badger.halt();

	return this->parent;
}