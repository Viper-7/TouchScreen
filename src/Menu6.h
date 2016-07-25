#ifndef MENU6_H
#define MENU6_H

#include "TouchScreen.h"
#include "Menu.h"

class Menu6 : public Menu {
	public:
		Menu6();

		void
			drawFrame(),
			loop(TS_Point p);
		
};

#endif
