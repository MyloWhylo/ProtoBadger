#ifndef __IMAGEMENUITEM_HPP
#define __IMAGEMENUITEM_HPP

#include "Menu.hpp"
#include "CustomBadger.hpp"

class ImageMenuItem : public MenuItem {
  private:
	MyloImage* img;

  public:
	ImageMenuItem(const char* name, Menu* parentMenu = nullptr, MyloImage* image = nullptr);
   Menu* handleInteraction();
};

#endif