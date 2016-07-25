#include "SPI.h"
#include <Adafruit_GFX_AS.h>    // Core graphics library, with extra fonts.
#include <Adafruit_ILI9341_STM.h> // STM32 DMA Hardware-specific library
#include <XPT2046_Touchscreen.h>
#include <TouchScreen.h>

TouchScreen ts2 = TouchScreen();

void setup() { 
  ts2.begin();
}
void loop() {
  ts2.loop();
}

