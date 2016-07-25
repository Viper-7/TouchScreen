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

		static const char margin = 0;
		static const char lineHeight = 20;
		static const uint16 maxX = 320;
		static const uint16 maxY = 240;

		char wastouched[4] = {0,0,0,0};
		char outputState = 0;
		char selected = 0;
		char oldselection = 4;
		char highlightButton = 0;
};

#endif