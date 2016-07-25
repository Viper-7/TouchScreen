#include "Menu.h"
#include "Menu3.h"

Menu3::Menu3() {

}

/**
 * External method used by the emergency stop function
 **/
void Menu3::stop() {
	outputState = 0;
	if(ts->curmenu == 1) {
		drawButtons();
	}
}

/**
 * Draw the bulk of the interface
 **/
void Menu3::drawFrame() {
  drawTitle("Ch 3");

  ts->tft.drawString("Duty", 10, 55, 2);
  ts->tft.drawRightString("85", 180, 30, 7);
  ts->tft.drawString("%", 180, 55, 4);

/*  ts->tft.drawString("??", 10, 110, 2);
  ts->tft.drawRightString("", 180, 85, 7);
  ts->tft.drawString("", 180, 110, 4);

  ts->tft.drawString("??", 10, 165, 2);
  ts->tft.drawRightString("", 180, 140, 7);
  ts->tft.drawString("", 180, 165, 4);
*/

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
void Menu3::touchArrow(char index) {
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
      tmpmod = interval + (mod*5);
      if(tmpmod < 0) tmpmod = 0;
      if(tmpmod > 1999) tmpmod = 1999;
      interval = (uint16)tmpmod;
      drawInterval(); 
      break;
    case 1: 
      tmpmod = duty + mod;
      if(tmpmod < 0) tmpmod = 0;
      if(tmpmod > 100) tmpmod = 100;
      duty = (uint16)tmpmod;
      drawDuty(); 
      break;
    case 2: 
      tmpmod = period + (mod*5);
      if(tmpmod < 0) tmpmod = 0;
      if(tmpmod > 1999) tmpmod = 1999;
      period = (uint16)tmpmod;
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
void Menu3::touchOn() {
  if (outputState == 0) {
    outputState = 1;
	Serial.print("S3G");
    drawButtons();
  }
}

/**
 * Off button at the botton of the screen, disable output.
 **/
void Menu3::touchOff() {
  if (outputState == 1) {
    outputState = 0;
	Serial.print("S3S");
    drawButtons();
  }
}

/**
 * Draw On/Off buttons at the bottom of the screen in their current state
 **/
void Menu3::drawButtons() {
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
void Menu3::drawSelector() {
  char index = 2 - ((2 + selected) % 3);
  ts->tft.fillRect(220, 25, 19, 170, 0);
  ts->tft.drawTriangle(220, 55 + (index * 55), 235, 65 + (index * 55), 235, 45 + (index * 55), textColor);
}

/**
 * Update the duty value on screen
 **/
void Menu3::drawDuty() {
  char tmp[4];
  ts->tft.setTextColor(textColor);
  if (duty != oldduty)
    ts->tft.fillRect(70, 140, 110, 50, 0);
  sprintf(tmp, "%lu", duty);
  ts->tft.drawRightString(tmp, 180, 140, 7);
  oldduty = duty;
}

/**
 * Update the interval value on screen
 **/
void Menu3::drawInterval() {
  char tmp[5];
  ts->tft.setTextColor(textColor);
  if (interval != oldinterval)
    ts->tft.fillRect(60, 30, 120, 50, 0);
  sprintf(tmp, "%lu", interval);
  ts->tft.drawRightString(tmp, 180, 30, 7);
  oldinterval = interval;
}

/**
 * Update the period value on screen
 **/
void Menu3::drawPeriod() {
  char tmp[5];
  ts->tft.setTextColor(textColor);
  if (period != oldperiod)
    ts->tft.fillRect(60, 85, 120, 50, 0);
  sprintf(tmp, "%lu", period);
  ts->tft.drawRightString(tmp, 180, 85, 7);
  oldperiod = period;
}

/**
 * Main input handler
 **/
void Menu3::loop(TS_Point p) {
}