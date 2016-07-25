#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H
 
#include <Adafruit_GFX_AS.h>    // Core graphics library, with extra fonts.
#include <Adafruit_ILI9341_STM.h> // STM32 DMA Hardware-specific library
#include <XPT2046_Touchscreen.h>

#define TS_CS PA3
#define TS_IRQ PA4

#define TFT_CS         PA0
#define TFT_DC         PA2
#define TFT_RST        PA1

class TouchScreen {
	public:
        TouchScreen();
        ~TouchScreen();
		char curmenu = -1;
		char dimstate;
		
		void
			setMenu(char menu),
			stopAll(),
			updateStatus();
			
		virtual void
			begin(),
			loop();
		
		XPT2046_Touchscreen ts = XPT2046_Touchscreen(TS_CS, TS_IRQ);
		Adafruit_ILI9341_STM tft = Adafruit_ILI9341_STM(TFT_CS, TFT_DC, TFT_RST);
	
	private:
		static const char brightness = 140;
		static const char minbrightness = 4;

		unsigned long dimtime;
};

#endif