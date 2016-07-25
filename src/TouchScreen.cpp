#include "TouchScreen.h" //include the declaration for this class

#include "Menu0.h"
#include "Menu1.h"
#include "Menu2.h"
#include "Menu3.h"
#include "Menu5.h"
#include "Menu6.h"

Menu0 menu0 = Menu0();
Menu1 menu1 = Menu1();
Menu2 menu2 = Menu2();
Menu3 menu3 = Menu3();
Menu5 menu5 = Menu5();
Menu6 menu6 = Menu6();

TouchScreen::TouchScreen() {

}

TouchScreen::~TouchScreen() {
	
}

/**
 * Stop all outputs immediately (emergency stop)
 **/
void TouchScreen::stopAll() {
	menu1.stop();
	menu2.stop();
	menu3.stop();
}

/**
 * Enable the backlight and pass a handle to the menus so they can update the screen
 */
void TouchScreen::begin() {
  // Load interval, duty & period
  Serial.begin(19200);
  
  analogWrite(PB9, 255 - brightness);

  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setRotation(1);
  digitalWrite(TFT_CS, HIGH);
  ts.begin();
  
  dimtime = millis() + 60000;
  
  menu0.begin(*this);
  menu1.begin(*this);
  menu2.begin(*this);
  menu3.begin(*this);
  menu5.begin(*this);
  menu6.begin(*this);
  
  setMenu(0);
}

/**
 * Main Loop
 **/
void TouchScreen::loop() {
	unsigned long now = millis();
	
	// Fetch touch data
	digitalWrite(TFT_CS, HIGH);		// Hack since the tft lib doesnt correctly de-assert CS when its not using the LCD
	TS_Point p = ts.getPoint();
	digitalWrite(TFT_CS, LOW);
	TS_Point d = TS_Point(0,0,0);	// Dummy point with 0 pressure to allow buttonUp events to fire manually
	
	// Emergency Stop function. If the display is dimmed, and we're on the home screen, pass the touch through to 
	// the menu before anything else, since it wont normally be called until the display reaches full brightness.
	if(dimstate == 1 && curmenu == 0 && p.z > 20) {
		menu0.loop(p);
		menu0.loop(d);
	}
	
	// Detect recent touches (even gentle ones) to delay the automatic backlight
	if (p.z > 20) {
		dimtime = now + 60000;
		if(dimstate == 1) {
			for(int i=minbrightness;i<brightness;i++) {
				analogWrite(PB9, 255 - i);
				delay(5);
			}
			dimstate = 0;
			return;
		}
	}
	
	// Back button is in the top left of all screens except the home menu
	if(curmenu > 0) {
		uint16 calcX, calcY;

		if (p.z > 200) {
			calcX = (p.y / 10) - 25;
			calcY = 240 - ((p.x / 14) - 25);

			if (calcX < 70 && calcY < 30) {
				setMenu(0);
				return;
			}
		}
	}
	
	// Process each menu's input handler if it's active
	switch(curmenu) {
		case 0:
			menu0.loop(p);
			break;
		case 1:
			menu1.loop(p);
			break;
		case 2:
			menu2.loop(p);
			break;
		case 3:
			menu3.loop(p);
			break;
		case 5:
			menu5.loop(p);
			break;
		case 6:
			menu6.loop(p);
			break;
	}
	
	// If the screen isn't being touched, check if we should dim the display
	if (p.z < 200) {
		if(now > dimtime && dimstate == 0) {
			for(int i=brightness;i>minbrightness;i--) {
				analogWrite(PB9, 255-i);
				delay(10);
			}
			dimstate = 1;
		}
	}
}

/**
 * Update the status bar on the home screen, code located here because it uses information from every screen.
 **/
void TouchScreen::updateStatus() {
  char tmp[55];
  
  tft.setTextColor(ILI9341_GREEN);
  
  sprintf(tmp, "F: %lu/%lums %lu%%  T: %lu%% %lu%%  V: %lu-%lu %lu%%", menu1.period, menu1.interval, menu1.duty, menu2.interval, menu2.period, menu3.interval, menu3.period, menu3.duty);
  tft.drawRawString(tmp, 25, 225, 1);
}

/**
 * Change menu to the selected index and redraw the screen
 **/
void TouchScreen::setMenu(char menu) {
  tft.fillScreen(ILI9341_BLACK);

  if(curmenu != menu) {
    switch(menu) {
      case 0:
        menu0.drawFrame();
		updateStatus();
        break;
      case 1:
        menu1.drawFrame();
        break;
      case 2:
        menu2.drawFrame();
        break;
      case 3:
        menu3.drawFrame();
        break;
      case 5:
        menu5.drawFrame();
        break;
      case 6:
        menu6.drawFrame();
        break;
      default:
        menu = 0;
        menu0.drawFrame();
        break;
    }
    
    curmenu = menu;
  }
}

