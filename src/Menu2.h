#ifndef MENU2_H
#define MENU2_H

#include "TouchScreen.h"
#include "Menu.h"

class Menu2 : public Menu {
	public:
		Menu2();
		
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
			drawManualButton(),
			stop();
		
		uint16 interval = 80;
		uint16 period = 12;
		uint16 duty = 25;

		uint16 oldinterval = 0;
		uint16 oldperiod = 0;
		uint16 oldduty = 100;
		
		char manualActive = 0;
		unsigned long manualDisable = 0;
};

#endif
