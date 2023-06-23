#include "../headers/ImageMenuItem.hpp"

#include "../headers/CustomBadger.hpp"

using namespace pimoroni;
extern MyloBadger badger;

ImageMenuItem::ImageMenuItem(const char* name, Menu* parentMenu, MyloImage* image) : MenuItem(name, parentMenu) {
	this->img = image;
}

Menu* ImageMenuItem::handleInteraction() {
	badger.drawMyloImage(*(this->img));
	badger.led(0);
	badger.halt();
	return this->parent;
}
