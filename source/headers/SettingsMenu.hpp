#ifndef __SETTINGSMENU_HPP
#define __SETTINGSMENU_HPP

#include "Menu.hpp"

class IntegerSetting : public Menu {
  protected:
	uint8_t val;
	uint8_t oldVal;

	char prefix;

	void drawTitleBar();
	void drawNavBar();
	void drawMenuOptions();

  public:
  	IntegerSetting(const char* menuName, char cmdPrefix, uint8_t initialValue = 0, Menu* parent = nullptr);
	Menu* handleInteraction();

	void up();
	void down();

	Menu* click();
	Menu* back();

};

#endif