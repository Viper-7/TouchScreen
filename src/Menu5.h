#ifndef MENU5_H
#define MENU5_H

#include "TouchScreen.h"
#include "Menu.h"

class Menu5 : public Menu {
	public:
		Menu5();

		void
			drawFrame(),
			loop(TS_Point p);
		
};

#endif
