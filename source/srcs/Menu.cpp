#include "../headers/Menu.hpp"

#include "../headers/CustomBadger.hpp"
#include "string.h"

using namespace pimoroni;
extern MyloBadger badger;

/**
 * \brief Construct a new MenuItem
 * \param name The name of the item (what's shown when display() is called)
 * \param parent The menu that this item is a parent of
 */
MenuItem::MenuItem(const char* name, Menu* parent) {
	this->name = name;
	this->parent = parent;
}

/**
 * \brief Sets the parent of this MenuItem
 * \param *newParent The new parent Menu
 */
void MenuItem::setParent(Menu* newParent) {
	this->parent = newParent;
}

/**
 * \brief Handles the click() interaction
 * \returns A pointer to the Menu that should be displayed. This is used to update the "state machine" that drives the menu interface, and makes submenus possible.
 */
Menu* MenuItem::handleInteraction() {
	return this->parent;
}

/**
 * \brief Displays the name of the parameter
 * \param x X position to display the name at
 * \param y Y position to display the name at
 * \param selected Whether or not this item is selected
 * \param scale The scale to draw the text at
 * \returns Number of vertical pixels taken up
 */
int MenuItem::display(int32_t x, int32_t y, bool selected, float scale) {
	int offsetWidth = badger.measure_text("> ", scale);
	if (selected) {
		badger.text("> ", x, y, scale);
	} else {
		badger.text("  ", x, y, scale);
	}
	badger.text(this->name, x + offsetWidth, y, scale);
	return 8 * scale;
}

Menu::Menu(const char* menuName, int entries, MenuItem** childItems, Menu* parent) {
	this->name = menuName;
	this->size = entries;
	this->parentMenu = parent;
	this->items = childItems;
}

Menu::~Menu() {}

/**
 * @brief Draws the title bar of a Menu
 *
 */
void Menu::drawTitleBar() {
	badger.text(this->name, 0, 0, 3.0f);
	char idxText[6];
	int len = snprintf(idxText, sizeof(char) * 5, "%d/%d", this->index + 1, this->size);
	idxText[len] = '\0';

	int textSize = badger.measure_text(idxText, 3.0f);
	badger.text(idxText, DISP_WIDTH - textSize, 0, 3.0f);
}

/**
 * @brief Draws the nav bar (bottom bar) of a Menu
 *
 */
void Menu::drawNavBar() {
	badger.pen(0);
	badger.rectangle(0, DISP_HEIGHT - 10, DISP_WIDTH - 1, DISP_HEIGHT - 1);
	badger.pen(15);

	int_fast16_t backPos = (DISP_WIDTH / 2) - (badger.measure_text("Back") / 2);
	int_fast16_t selPos = 40 - (badger.measure_text("Select") / 2);

	badger.text("Select", selPos, DISP_HEIGHT - 9);

	if (this->parentMenu != nullptr) {
		badger.text("Back", backPos, DISP_HEIGHT - 9);
	}

	badger.pen(0);
}

/**
 * @brief Draws the central section of a Menu (usually contains the options)
 *
 */
void Menu::drawMenuOptions() {
	int currentHeight = 25;
	int startingIndex = 0;

	// Figure out what item to start drawing on
	if (this->size - this->index <= 2) {  // If we're at the end of the list, draw the 5 last items
		startingIndex = this->size - 5;
	} else {  // Otherwise, start drawing from 2 before the current item
		startingIndex = this->index - 2;
	}

	if (startingIndex < 0) startingIndex = 0;  // Make sure we properly draw the first items in the list

	// Iterate over the items, being mindful of the height we're drawing at.
	for (int ii = startingIndex; ii < startingIndex + 5 && ii < this->size; ii++) {
		if (DISP_HEIGHT - 18 <= currentHeight) break;
		currentHeight += this->items[ii]->display(0, currentHeight, (ii == this->index), 2);
	}
}

void Menu::clear() {
	badger.pen(15);
	badger.clear();
	badger.pen(0);
}

void Menu::display() {
	badger.led(255);  // Turn the LED on so we know it's working

	// clear the display
	this->clear();

	// If we've got an error, draw it
	if (this->errorState != NONE) this->displayError();

	// Draw all parts of menu
	this->drawTitleBar();
	this->drawNavBar();
	this->drawMenuOptions();

	// Update the panel
	badger.update(true);
	badger.led(85);

	// If we've got few options, we can update faster.
	if (this->size <= 5) badger.update_speed(3);
}

void Menu::displayError() {
	badger.font("bitmap6");
	badger.pen(0);

	switch (this->errorState) {
		case RET_NULLPTR:
			badger.text("MenuItem returned nullptr!", 0, DISP_HEIGHT - 20);
			break;

		default:
			break;
	}

	badger.font("bitmap8");
}

void Menu::down() {
	this->index++;
	if (this->index >= this->size) {
		this->index = this->size - 1;
	}
}

void Menu::up() {
	this->index--;
	if (this->index < 0) {
		this->index = 0;
	}
}

Menu* Menu::click() {
	Menu* retVal = this->items[this->index]->handleInteraction();
	if (retVal == nullptr) {
		this->errorState = RET_NULLPTR;
		return this;
	}

	badger.update_speed(2);
	return retVal;
}

Menu* Menu::back() {
	badger.update_speed(2);
	if (this->parentMenu) {
		return this->parentMenu;
	} else
		return this;
}

void Menu::action() {}

void Menu::setParent(Menu* parent) {
	this->parentMenu = parent;
}

Menu* Menu::getParent() {
	return this->parentMenu;
}