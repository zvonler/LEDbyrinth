/*
    Copyright 2019 Zach Vonler

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "BoardController.hpp"
#include "Orientation.hpp"
#include "Screensaver.hpp"

#include <avr/sleep.h>
#include <avr/wdt.h>

PictureFrame pictureFrame;
BoardController controller(pictureFrame.matrix(), accelerometer, levels);

void setup() {
  // Required because powering AREF pin - analogRead will short otherwise
  analogReference(EXTERNAL);

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  pictureFrame.enable();
  controller.reset();
}

uint32_t lastBoardChangeTm = 0;
uint32_t screensaverTimeout = 10000; // millis
uint32_t sleepTimeout = 20000; // millis

void loop() {

  bool didChange = controller.update();
  if (didChange) {
    lastBoardChangeTm = millis();

  } else if (millis() - lastBoardChangeTm > screensaverTimeout) {
    Orientation orientation(0.5);

    while (!orientation.changed()) {
      if (millis() - lastBoardChangeTm < sleepTimeout) {
        pictureFrame.screenSaverUpdate();
      } else {
        // Deep sleep
        pictureFrame.disable();
        while (!orientation.changed()) {
          delay(2000);
          // Disabled powerSave while testing FET
          //        powerSave();
        }
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        pictureFrame.enable();
        digitalWrite(LED_BUILTIN, LOW);
        break;
      }
    }
    controller.setLevel(1);
    lastBoardChangeTm = millis();
  }
}

/*
   Power save code from http://www.gammon.com.au/power
*/

// watchdog intervals
// sleep bit patterns for WDTCSR
enum
{
  WDT_16_MS  =  0b000000,
  WDT_32_MS  =  0b000001,
  WDT_64_MS  =  0b000010,
  WDT_128_MS =  0b000011,
  WDT_256_MS =  0b000100,
  WDT_512_MS =  0b000101,
  WDT_1_SEC  =  0b000110,
  WDT_2_SEC  =  0b000111,
  WDT_4_SEC  =  0b100000,
  WDT_8_SEC  =  0b100001,
};  // end of WDT intervals enum

// watchdog interrupt
ISR (WDT_vect)
{
  wdt_disable();  // disable watchdog
}  // end of WDT_vect

void powerSave() {
  // clear various "reset" flags
  MCUSR = 0;
  // allow changes, disable reset
  WDTCSR = bit (WDCE) | bit (WDE);
  // set interrupt mode and interval
  WDTCSR = bit (WDIE) | WDT_2_SEC;
  wdt_reset();  // pat the dog

  // disable ADC
  byte old_ADCSRA = ADCSRA;
  ADCSRA = 0;

  // disable various modules
  byte old_PRR = PRR;
  PRR = 0xFF;

  noInterrupts();           // timed sequence follows

  set_sleep_mode (SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  // turn off brown-out enable in software
  MCUCR = bit (BODS) | bit (BODSE);
  MCUCR = bit (BODS);
  interrupts();             // guarantees next instruction executed
  sleep_cpu();

  // cancel sleep as a precaution
  sleep_disable();

  PRR = old_PRR;
  ADCSRA = old_ADCSRA;
}
