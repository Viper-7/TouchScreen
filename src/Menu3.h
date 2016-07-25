#ifndef MENU3_H
#define MENU3_H

#include "TouchScreen.h"
#include "Menu.h"

class Menu3 : public Menu {
	public:
		Menu3();

		void
			drawButtons(),
			drawSelector(),
			drawDuty(),
			drawInterval(),
			drawPeriod(),
			drawFrame(),
			touchOn(),
			touchOff(),
			touchArrow(char index),
			loop(TS_Point p),
			stop();
		
		uint16 interval = 850;
		uint16 period = 125;
		uint16 duty = 25;

		uint16 oldinterval = 0;
		uint16 oldperiod = 0;
		uint16 oldduty = 100;
};

#endif
