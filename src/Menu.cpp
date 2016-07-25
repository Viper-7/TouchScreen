#include "Menu.h"

Menu::Menu() {
	
}

void Menu::begin(TouchScreen &ts) {
	this->ts = &ts;
}

void Menu::loop(TS_Point p) {

}

void Menu::touchArrow(char index) {

}

void Menu::drawArrow(char index, uint16 color) {
  switch(index) {
    case 0:
      ts->tft.fillTriangle(280, 30, 310, 50, 250, 50, color);
      ts->tft.fillTriangle(280, 40, 310, 60, 250, 60, color);
      break;
    case 1:
      ts->tft.fillTriangle(280, 75, 310, 115, 250, 115, color);
      break;
    case 2:
      ts->tft.fillTriangle(280, 185, 310, 145, 250, 145, color);
      break;
    case 3:
      ts->tft.fillTriangle(280, 220, 310, 200, 250, 200, color);
      ts->tft.fillTriangle(280, 230, 310, 210, 250, 210, color);
      break;
  }
}

void Menu::drawTitle(char* content) {
  uint16 menuColor;
  char margin2 = margin * 2;
  menuColor = textColor;

  ts->tft.drawRect(margin, margin, maxX - margin2, maxY - margin2, menuColor);
  ts->tft.drawLine(margin, margin + lineHeight, maxX - margin, margin + lineHeight, menuColor);
  ts->tft.setCursor(5, 5);
  ts->tft.setTextColor(menuColor);
  ts->tft.setTextWrap(1);
  ts->tft.drawCentreString(content, 30 + (maxX / 2), 2, 2);
}

void Menu::drawBackButton() {
  uint16 buttonColor = ILI9341_YELLOW;
  ts->tft.fillRect(0,0,70,20,buttonColor);
  ts->tft.fillRect(0,0,60,30,buttonColor);
  ts->tft.fillRoundRect(0,0,70,30,5,buttonColor);
  ts->tft.setTextColor(0);
  ts->tft.drawString("Back", 6, 4, 4);
}
