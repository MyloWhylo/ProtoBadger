#ifndef __MENU_HPP
#define __MENU_HPP

#include <stdbool.h>
#include <stdint.h>

#include "CustomBadger.hpp"

#define NUM_VISIBLE_ENTRIES 5

typedef enum MenuErrorType {
	NONE,
	RET_NULLPTR
} MenuErrorType;

class Menu;
class MenuItem;

class MenuItem {
  public:
	const char* name;
	const char* customFunction;
	Menu* parent;
	MenuItem(const char* name, Menu* parent = nullptr);
	~MenuItem();

	virtual Menu* handleInteraction();

	void setParent(Menu* newParent);
	void setImage(const MyloImage* img);

	virtual int display(int32_t x, int32_t y, bool selected, float scale = 1.0f);
};

class Menu {
  protected:
	const char* name;
	int index = 0;
	int size;
	Menu* parentMenu;
	MenuItem** items;
	MenuErrorType errorState = NONE;

	void displayError();
	virtual void clear();
	virtual void drawTitleBar();
	virtual void drawNavBar();
	virtual void drawMenuOptions();

  public:
	Menu(const char* menuName, int entries, MenuItem** childItems, Menu* parent = nullptr);
	~Menu();
	void setParent(Menu* parent);
	Menu* getParent();

	virtual void display();
	virtual void up();
	virtual void down();
	virtual void action();

	virtual Menu* click();
	virtual Menu* back();
};

#endif