#include "Menu.h"
#include "Menu0.h"

Menu0::Menu0() {

}

/**
 * Draw the tiles on the home screen
 **/
void Menu0::drawButtons() {
  uint16 menuColor;

  menuColor = highlightButton == 1 ? highlightColor : textColor;
  ts->tft.drawRect(10,40,90,60,menuColor);
  ts->tft.setTextColor(menuColor);
  ts->tft.drawCentreString("Ch 1",55,60,4);

  menuColor = highlightButton == 2 ? highlightColor : textColor;
  ts->tft.drawRect(115,40,90,60,menuColor);
  ts->tft.setTextColor(menuColor);
  ts->tft.drawCentreString("Ch 2",160,60,4);

  menuColor = highlightButton == 3 ? highlightColor : textColor;
  ts->tft.drawRect(220,40,90,60,menuColor);
  ts->tft.setTextColor(menuColor);
  ts->tft.drawCentreString("Ch 3",265,60,4);


  menuColor = highlightButton == 4 ? highlightColor : ILI9341_RED;
  ts->tft.drawRect(10,130,90,60,menuColor);
  ts->tft.setTextColor(menuColor);
  ts->tft.drawCentreString("Stop",55,150,4);

  menuColor = highlightButton == 5 ? highlightColor : textColor;
  ts->tft.drawRect(115,130,90,60,menuColor);
  ts->tft.setTextColor(menuColor);
  ts->tft.drawCentreString("Stats",160,150,4);

  menuColor = highlightButton == 6 ? highlightColor : textColor;
  ts->tft.drawRect(220,130,90,60,menuColor);
  ts->tft.setTextColor(menuColor);
  ts->tft.drawCentreString("Setup",265,150,4);
}

/**
 * Draw the frame of the interface.
 * Don't use the drawTitle helper here because leaves room for the back button which will not appear on this screen.
 **/
void Menu0::drawFrame() {
  char margin2 = margin * 2;
  uint16 menuColor;
  menuColor = textColor;

  ts->tft.drawRect(margin, margin, maxX - margin2, maxY - margin2, menuColor);
  ts->tft.drawLine(margin, margin + lineHeight, maxX - margin, margin + lineHeight, menuColor);
  ts->tft.drawLine(margin, maxY - (margin + lineHeight), maxX - margin, maxY - (margin + lineHeight), menuColor);
  ts->tft.setCursor(5, 5);
  ts->tft.setTextColor(menuColor);
  ts->tft.setTextWrap(1);
  ts->tft.drawCentreString("Viper-7's Motor Controller", maxX / 2, 2, 2);
  drawButtons();
}

/**
 * Input handler for the home screen. Make tiles glow on touch, and navigate to the selected tile when the touch is released
 **/ 
void Menu0::loop(TS_Point p) {
  uint16 calcX, calcY;
  unsigned long now = millis();
  char oldhighlight;
  
  if (p.z > 200) {
	// Touched
    calcX = (p.y / 10) - 25;
    calcY = 240 - ((p.x / 14) - 25);

	// Ignore presses on the title bar
    if(calcY < 40 || calcY > 200) return;
    
	if(calcY < 120) {
	  // Top row
      oldhighlight = highlightButton;
      if(calcX < 105) {
        highlightButton = 1;
      } else if(calcX < 210) {
        highlightButton = 2;
      } else {
        highlightButton = 3;
      }
    } else {
	  // Bottom Row
      if(calcX < 105) {
        highlightButton = 4;
      } else if(calcX < 210) {
        highlightButton = 5;
      } else {
        highlightButton = 6;
      }
    }
	
	// Redraw the tiles if needed
    if(oldhighlight != highlightButton) {
        drawButtons();
    }
  } else {
    if(highlightButton > 0) {
      oldhighlight = highlightButton;
      highlightButton = 0;
      
	  // If we detected a stop press, action it immediately.
	  if(oldhighlight == 4) {
		  Serial.write("STOP\n");
		  ts->stopAll();
		  highlightButton = 0;
		  drawButtons();
	  }
	  
	  // Ignore the touch that woke the display
	  if(ts->dimstate == 0) {
		  // Navigate to the selected menu
		  switch(oldhighlight) {
			case 1:
			  ts->setMenu(1);
			  break;
			case 2:
			  ts->setMenu(2);
			  break;
			case 3:
			  ts->setMenu(3);
			  break;
			case 5:
			  ts->setMenu(5);
			  break;
			case 6:
			  ts->setMenu(6);
			  break;
		  }
		}
    }
  }
}