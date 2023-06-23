#ifndef __SUBMENUITEM_HPP
#define __SUBMENUITEM_HPP

#include "Menu.hpp"

class SubMenuItem : public MenuItem {
  private:
	Menu* child;

  public:
	SubMenuItem(const char* name, Menu* parentMenu = nullptr, Menu* childMenu = nullptr);
	void setChild(Menu* newChild);
	Menu* handleInteraction();
};

#endif