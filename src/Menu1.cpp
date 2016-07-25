#include "Menu.h"
#include "Menu1.h"

Menu1::Menu1() {

}

void Menu1::stop() {
	outputState = 0;
	if(ts->curmenu == 1) {
		drawButtons();
	}
}

void Menu1::drawFrame() {
  uint16 menuColor = textColor;

  drawTitle("Ch 1");

  ts->tft.drawString("Interval", 10, 55, 2);
  drawInterval();
  ts->tft.drawString("ms", 180, 55, 4);

  ts->tft.drawString("Period", 10, 110, 2);
  drawPeriod();
  ts->tft.drawString("ms", 180, 110, 4);

  ts->tft.drawString("Duty", 10, 165, 2);
  drawDuty();
  ts->tft.drawString("%", 180, 165, 4);

  ts->tft.drawLine(240, margin + lineHeight, 240, maxY - (margin * 2), menuColor);

  for(char i=0;i<4;i++) {
    drawArrow(i, menuColor);
  }

  ts->tft.fillRect(255, 128, 50, 4, shadeColor);

  drawBackButton();
  drawButtons();
  drawSelector();
}

void Menu1::touchArrow(char index) {
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
      Serial.print("S1A=");
	  Serial.print(interval, HEX);
	  Serial.print("\n");
	  drawInterval(); 
      break;
    case 1: 
      tmpmod = duty + mod;
      if(tmpmod < 0) tmpmod = 0;
      if(tmpmod > 100) tmpmod = 100;
      duty = (uint16)tmpmod;
      Serial.print("S1B=");
	  Serial.print(duty, HEX);
	  Serial.print("\n");
      drawDuty(); 
      break;
    case 2: 
      tmpmod = period + (mod*5);
      if(tmpmod < 0) tmpmod = 0;
      if(tmpmod > 1999) tmpmod = 1999;
      period = (uint16)tmpmod;
      Serial.print("S1C=");
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

void Menu1::touchOn() {
  if (outputState == 0) {
    outputState = 1;
	Serial.print("S1G");
    drawButtons();
  }
}

void Menu1::touchOff() {
  if (outputState == 1) {
    outputState = 0;
	Serial.print("S1S");
    drawButtons();
  }
}

void Menu1::drawButtons() {
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

void Menu1::drawSelector() {
  char index = 2 - ((2 + selected) % 3);
  ts->tft.fillRect(220, 25, 19, 170, 0);
  ts->tft.drawTriangle(220, 55 + (index * 55), 235, 65 + (index * 55), 235, 45 + (index * 55), textColor);
}

void Menu1::drawDuty() {
  char tmp[4];
  ts->tft.setTextColor(textColor);
  if (duty != oldduty)
    ts->tft.fillRect(70, 140, 110, 50, 0);
  sprintf(tmp, "%lu", duty);
  ts->tft.drawRightString(tmp, 180, 140, 7);
  oldduty = duty;
}

void Menu1::drawInterval() {
  char tmp[5];
  ts->tft.setTextColor(textColor);
  if (interval != oldinterval)
    ts->tft.fillRect(60, 30, 120, 50, 0);
  sprintf(tmp, "%lu", interval);
  ts->tft.drawRightString(tmp, 180, 30, 7);
  oldinterval = interval;
}

void Menu1::drawPeriod() {
  char tmp[5];
  ts->tft.setTextColor(textColor);
  if (period != oldperiod)
    ts->tft.fillRect(60, 85, 120, 50, 0);
  sprintf(tmp, "%lu", period);
  ts->tft.drawRightString(tmp, 180, 85, 7);
  oldperiod = period;
}

void Menu1::loop(TS_Point p) {
  uint16 calcX, calcY;
  unsigned long now = millis();
  
  if (p.z > 200) {
    calcX = (p.y / 10) - 25;
    calcY = 240 - ((p.x / 14) - 25);

    if (calcX < 70 && calcY < 30) {
      ts->setMenu(0);
      return;
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
      if (calcY < 20) return; // Header
      if (calcY < 80) selected = 0;
      else if (calcY < 135) selected = 2;
      else selected = 1;
      if (selected != oldselection)
        drawSelector();
      oldselection = selected;
    }
  } else {
    
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