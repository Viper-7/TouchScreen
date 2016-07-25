#ifndef MENU_H
#define MENU_H

#include "TouchScreen.h"

class Menu {
	public:
		Menu();

		virtual void
			drawFrame(),
			drawTitle(char* content),
			drawBackButton(),
			drawArrow(char index, uint16 color),
			touchArrow(char index),
			loop(TS_Point p),
			begin(TouchScreen &ts);

		TouchScreen *ts;
		
		static const uint16 borderColor = 0B0000011111100000;
		static const uint16 highlightColor = 0B1100011111111000;
		static const uint16 shadeColor = 0B0001010111100010;
		static const uint16 textColor = 0B0000011111100000;
		
		static const uint16 redColor = ILI9341_RED;
		static const uint16 greenColor = ILI9341_GREEN;

		static const char margin = 0;		// Margin between edge of screen and border
		static const char lineHeight = 20;	// Height of title & status bar
		static const uint16 maxX = 320;	// Screen X
		static const uint16 maxY = 240;	// Screen Y

		char wastouched[4] = {0,0,0,0};	// Used for the up/down arrows on most menus
		char outputState = 0;					// Current output status
		char selected = 0;							// Index of currently selected value (duty/interval/period)
		char oldselection = 4;					// Temporary holder of last selected value
};

#endif