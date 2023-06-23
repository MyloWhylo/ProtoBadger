#ifndef __CLEARMENUITEM_HPP
#define __CLEARMENUITEM_HPP

#include "Menu.hpp"

class ClearMenuItem : public MenuItem {

  public:
	ClearMenuItem(const char* name, Menu* parentMenu = nullptr);
	Menu* handleInteraction();
};

#endif