#ifndef __MENUITEMS_HPP
#define __MENUITEMS_HPP

#include <stdio.h>

#include "../image.hpp"
#include "ClearMenuItem.hpp"
#include "ImageMenuItem.hpp"
#include "Menu.hpp"
#include "SettingsMenu.hpp"
#include "SubMenuItem.hpp"

void initializeMenu();

// Menu declaration
Menu* mainMenu = nullptr;
Menu* imageMenu = nullptr;
Menu* prootMenu = nullptr;

// Main Menu
SubMenuItem changeImage("Change Image", mainMenu, imageMenu);
ClearMenuItem clearScreen("Clear Screen", mainMenu);
SubMenuItem prootControl("Protogen Settings", mainMenu, prootMenu);
MenuItem* mainItems[] = {&changeImage, &clearScreen, &prootControl};

// Image Menu
ImageMenuItem badgeImage("Mylo Badge", imageMenu, (MyloImage*)&badgeTest);
ImageMenuItem testImage("Test Image", imageMenu, (MyloImage*)&IndyFurCon);
MenuItem* imageItems[] = {&badgeImage, &testImage};

// Protogen Menu

IntegerSetting prootEmoteMenu("Emotion", 'e', 0, prootMenu);
IntegerSetting prootBrightMenu("Brightness", 'b', 0, prootMenu);

SubMenuItem prootEmote("Emotion", prootMenu, &prootEmoteMenu);
SubMenuItem prootBright("Brightness", prootMenu, &prootBrightMenu);
MenuItem* prootItems[] = {&prootEmote, &prootBright};

void initializeMenu() {
	mainMenu = new Menu("Main Menu", std::size(mainItems), mainItems);
	imageMenu = new Menu("Change Image", std::size(imageItems), imageItems, mainMenu);
	prootMenu = new Menu("Protogen Control", std::size(prootItems), prootItems, mainMenu);

	changeImage.setChild(imageMenu);
	prootControl.setChild(prootMenu);
	prootEmoteMenu.setParent(prootMenu);
	prootBrightMenu.setParent(prootMenu);
}

#endif