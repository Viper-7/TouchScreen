#ifndef MENU0_H
#define MENU0_H

#include "TouchScreen.h"
#include "Menu.h"

class Menu0 : public Menu {
	public:
		Menu0();
		
		void
			drawButtons(),
			drawFrame(),
			loop(TS_Point p);
			
};

#endif
