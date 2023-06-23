#include "../headers/SubMenuItem.hpp"
#include "../headers/CustomBadger.hpp"

using namespace pimoroni;
extern MyloBadger badger;

SubMenuItem::SubMenuItem(const char* name, Menu* parentMenu, Menu* childMenu) : MenuItem(name, parentMenu) {
   this->child = childMenu;
}

void SubMenuItem::setChild(Menu* newChild){
   this->child = newChild;
}

Menu* SubMenuItem::handleInteraction() {
   return this->child;
}