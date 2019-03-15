/*
    Copyright 2019 Zach Vonler

    This file is part of LEDbyrinth.

    LEDbyrinth is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LEDbyrinth is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with LEDbyrinth.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _PICTUREFRAME_HPP_
#define _PICTUREFRAME_HPP_

#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>

#include "Screensaver.hpp"

#define BRIGHTNESS 30

// The pin the NeoPixels are on
#define SIGNAL_PIN 6

// The pin connected to the MOSFET gate
#define ENABLE_LED_PIN 7

class PictureFrame {
  public:

    PictureFrame() :
      _matrix(
        17, // width
        13, // height
        SIGNAL_PIN,  // control pin
        NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
        NEO_MATRIX_ROWS    + NEO_MATRIX_ZIGZAG,
        NEO_GRB            + NEO_KHZ800),
      _saver(_matrix),
      _enabled(true)
    {
      pinMode(ENABLE_LED_PIN, OUTPUT);

      disable();
    }

    Adafruit_NeoMatrix& matrix() {
      return _matrix;
    }

    void screenSaverUpdate() {
      _saver.update();
    }

    void disable() {
      if (!_enabled) return;

      digitalWrite(SIGNAL_PIN, LOW);
      pinMode(SIGNAL_PIN, INPUT);

      digitalWrite(ENABLE_LED_PIN, LOW);

      _enabled = false;
    }

    void enable() {
      if (_enabled) return;

      // Set pinMode - ctor does too but don't know when that runs
      pinMode(ENABLE_LED_PIN, OUTPUT);
      digitalWrite(ENABLE_LED_PIN, HIGH);

      // begin() method sets pinMode to OUTPUT
      _matrix.begin();
      _matrix.setBrightness(BRIGHTNESS);
      _matrix.clear();
      _matrix.show();

      _enabled = true;
    }

  private:
    Adafruit_NeoMatrix _matrix;
    Screensaver _saver;
    bool _enabled;
};

#endif
