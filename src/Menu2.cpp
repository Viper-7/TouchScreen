#include "Menu.h"
#include "Menu2.h"

Menu2::Menu2() {

}

/**
 * External method used by the emergency stop function
 **/
void Menu2::stop() {
	outputState = 0;
	if(ts->curmenu == 1) {
		drawButtons();
	}
}

/**
 * Draw the button for manual activation
 **/
void Menu2::drawManualButton() {
  if(manualActive) {
	  ts->tft.fillRect(30,150,180,40,highlightColor);
	  ts->tft.setTextColor(0);
	  ts->tft.drawCentreString("Manual", 120, 160, 4);
	  ts->tft.setTextColor(textColor);
  } else {
	  ts->tft.fillRect(30,150,180,40,0);
	  ts->tft.drawRect(30,150,180,40,textColor);
	  ts->tft.setTextColor(textColor);
	  ts->tft.drawCentreString("Manual", 120, 160, 4);
  }
}

/**
 * Draw the bulk of the interface
 **/
void Menu2::drawFrame() {
  drawTitle("Ch2");

  ts->tft.drawString("Duty 1", 10, 55, 4);
  drawInterval();
  ts->tft.drawString("%", 180, 65, 4);

  ts->tft.drawString("Duty 2", 10, 110, 4);
  drawPeriod();
  ts->tft.drawString("%", 180, 120, 4);

  drawManualButton();
  
  // Sidebar 
  ts->tft.drawLine(240, 20, 240, maxY, textColor);

  // Sidebar arrows
  for(char i=0;i<4;i++) {
    drawArrow(i, textColor);
  }

  // Line between arrows
  ts->tft.fillRect(255, 128, 50, 4, shadeColor);

  drawButtons();
  drawSelector();
  drawBackButton();
}

/**
 * Handle input for a touched arrow button (value change)
 **/
void Menu2::touchArrow(char index) {
  signed char mod;
  signed short int tmpmod;
  
  for(char i=0;i<4;i++) {
    if(i != index && wastouched[i] > 0) {
      drawArrow(i, borderColor);
      wastouched[i] = 0;
    }
  }
  if(!wastouched[index])
    drawArrow(index, highlightColor);

  switch(index) {
    case 0: mod = 5; break;
    case 1: mod = 1; break;
    case 2: mod = -1; break;
    case 3: mod = -5; break;
  }

  switch(selected) {
    case 0: 
      tmpmod = interval + mod;
      if(tmpmod < 0) tmpmod = 0;
      if(tmpmod > 1999) tmpmod = 1999;
      interval = (uint16)tmpmod;
      Serial.print("S2A=");
	  Serial.print(interval, HEX);
	  Serial.print("\n");
      drawInterval(); 
      break;
    case 1: 
      tmpmod = period + mod;
      if(tmpmod < 0) tmpmod = 0;
      if(tmpmod > 1999) tmpmod = 1999;
      period = (uint16)tmpmod;
      Serial.print("S2B=");
	  Serial.print(period, HEX);
	  Serial.print("\n");
      drawPeriod(); 
      break;
  }

  if(wastouched[index] < 2)
    delay(200);
  else
    delay(25);

  wastouched[index]++;
}

/**
 * On button at the botton of the screen, enable output.
 **/
void Menu2::touchOn() {
  if (outputState == 0) {
    outputState = 1;
	Serial.print("S2G");
    drawButtons();
  }
}

/**
 * Off button at the botton of the screen, disable output.
 **/
void Menu2::touchOff() {
  if (outputState == 1) {
    outputState = 0;
	Serial.print("S2S");
    drawButtons();
  }
}

/**
 * Draw On/Off buttons at the bottom of the screen in their current state
 **/
void Menu2::drawButtons() {
  ts->tft.fillRect(0, 200, 120, 40, redColor);
  ts->tft.fillRect(120, 200, 120, 40, greenColor);

  if (outputState == 1) {
    ts->tft.fillRect(5, 205, 110, 30, 0);
    ts->tft.setTextColor(0B1011100000000000);
    ts->tft.drawString("OFF", 35, 210, 4);
    ts->tft.setTextColor(0);
    ts->tft.drawString("ON", 160, 210, 4);
  } else {
    ts->tft.fillRect(125, 205, 110, 30, 0);
    ts->tft.setTextColor(0);
    ts->tft.drawString("OFF", 35, 210, 4);
    ts->tft.setTextColor(shadeColor);
    ts->tft.drawString("ON", 160, 210, 4);
  }
}

/**
 * Draw selection arrow for currently highlighted item
 **/
void Menu2::drawSelector() {
  char index = selected;
  ts->tft.fillRect(220, 25, 19, 170, 0);
  ts->tft.drawTriangle(220, 55 + (index * 55), 235, 65 + (index * 55), 235, 45 + (index * 55), textColor);
}

/**
 * Update the duty value on screen
 **/
void Menu2::drawDuty() {
  char tmp[4];
  ts->tft.setTextColor(textColor);
  if (duty != oldduty)
    ts->tft.fillRect(80, 140, 100, 50, 0);
  sprintf(tmp, "%lu", duty);
  ts->tft.drawRightString(tmp, 180, 140, 7);
  oldduty = duty;
}

/**
 * Update the interval value on screen
 **/
void Menu2::drawInterval() {
  char tmp[5];
  ts->tft.setTextColor(textColor);
  if (interval != oldinterval)
    ts->tft.fillRect(80, 40, 100, 50, 0);
  sprintf(tmp, "%lu", interval);
  ts->tft.drawRightString(tmp, 180, 40, 7);
  oldinterval = interval;
}

/**
 * Update the period value on screen
 **/
void Menu2::drawPeriod() {
  char tmp[5];
  ts->tft.setTextColor(textColor);
  if (period != oldperiod)
    ts->tft.fillRect(80, 95, 100, 50, 0);
  sprintf(tmp, "%lu", period);
  ts->tft.drawRightString(tmp, 180, 95, 7);
  oldperiod = period;
}

/**
 * Main input handler
 **/
void Menu2::loop(TS_Point p) {
	uint16 calcX, calcY;

  unsigned long now = millis();
  
  if (p.z > 200) {
	// Touched
    calcX = (p.y / 10) - 25;
    calcY = 240 - ((p.x / 14) - 25);

    if (calcX < 70 && calcY < 30) {
      ts->setMenu(0);
      return;
    }
	if(calcX > 30 && calcX < 210) {
		if(calcY > 150 && calcY < 190) {
			manualDisable = millis() + 100;
			if(!manualActive) {
				manualActive = 1;
				drawManualButton();
			}
		}
	}
    if (calcX > 240) {
      // Controls
      if (calcY < 30) return; // Header
      if (calcY < 70)
        touchArrow(0);
      else if (calcY < 125)
        touchArrow(1);
      else if (calcY < 185)
        touchArrow(2);
      else
        touchArrow(3);
    } else if (calcY > 175) {
      if (calcX < 120) {
        touchOff();
      } else {
        touchOn();
      }
    } else {
      if (calcY < 20 || calcY > 135) return; // Header
      if (calcY < 80) selected = 0;
      else selected = 1;
      if (selected != oldselection)
        drawSelector();
      oldselection = selected;
    }
  } else {
	if(manualActive && manualDisable < millis()) {
		manualActive = 0;
		drawManualButton();
	}
    
    if (wastouched[0] || wastouched[1] || wastouched[2] || wastouched[3]) {
      // Store interval, duty & period
    }

    for(char i=0;i<4;i++) {
      if(wastouched[i]) {
        drawArrow(i, borderColor);
        wastouched[i] = 0;
      }
    }
  }
}