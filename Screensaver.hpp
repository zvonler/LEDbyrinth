/*
  Adapted from https://github.com/johncarl81/neopixelplasma/blob/master/neopixelplasma.ino
    Multicolored Plasma for the Arduino Micro-Controller and NeoPixel Shield
    Copyright 2019 John Ericksen

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

#ifndef _SCREENSAVER_HPP_
#define _SCREENSAVER_HPP_

#include "PictureFrame.hpp"

class Screensaver {
  public:
    // Convenient 2D point structure
    struct Point {
      float x;
      float y;
    };

    Screensaver(Adafruit_NeoMatrix matrix) : _matrix(matrix) {
    }

    const float phaseIncrement = 0.08;  // Controls the speed of the moving points. Higher == faster. I like 0.08 ..03 change to .02
    const float colorStretch = 0.11;    // Higher numbers will produce tighter color bands. I like 0.11 . ok try .11 instead of .03

    void update() {
      _phase += phaseIncrement;

      // The two points move along Lissajious curves, see: http://en.wikipedia.org/wiki/Lissajous_curve
      // We want values that fit the LED grid: x values between 0..13, y values between 0..8 .
      // The sin() function returns values in the range of -1.0..1.0, so scale these to our desired ranges.
      // The phase value is multiplied by various constants; I chose these semi-randomly, to produce a nice motion.
      float maxX = float(_matrix.width()) / 2.0;
      float maxY = float(_matrix.height()) / 2.0;

      Point p1 = { (sin(_phase * 1.000) + 1.0) * maxX, (sin(_phase * 1.310) + 1.0) * maxY };
      Point p2 = { (sin(_phase * 1.770) + 1.0) * maxX, (sin(_phase * 2.865) + 1.0) * maxY };
      Point p3 = { (sin(_phase * 0.250) + 1.0) * maxX, (sin(_phase * 0.750) + 1.0) * maxY };

      byte row, col;

      // For each row...
      for ( row = 0; row < _matrix.height(); row++ ) {
        float row_f = float(row);  // Optimization: Keep a floating point value of the row number, instead of recasting it repeatedly.

        // For each column...
        for ( col = 0; col < _matrix.width(); col++ ) {
          float col_f = float(col);  // Optimization.

          // Calculate the distance between this LED, and p1.
          Point dist1 = { col_f - p1.x, row_f - p1.y };  // The vector from p1 to this LED.
          float distance1 = sqrt( dist1.x * dist1.x + dist1.y * dist1.y );

          // Calculate the distance between this LED, and p2.
          Point dist2 = { col_f - p2.x, row_f - p2.y };  // The vector from p2 to this LED.
          float distance2 = sqrt( dist2.x * dist2.x + dist2.y * dist2.y );

          // Calculate the distance between this LED, and p3.
          Point dist3 = { col_f - p3.x, row_f - p3.y };  // The vector from p3 to this LED.
          float distance3 = sqrt( dist3.x * dist3.x + dist3.y * dist3.y );

          // Warp the distance with a sin() function. As the distance value increases, the LEDs will get light,dark,light,dark,etc...
          // You can use a cos() for slightly different shading, or experiment with other functions. Go crazy!
          float color_1 = distance1;  // range: 0.0...1.0
          float color_2 = distance2;
          float color_3 = distance3;
          float color_4 = (cos( distance1 * distance2 * colorStretch )) + 2.0 * 0.5;

          // Square the color_f value to weight it towards 0. The image will be darker and have higher contrast.
          color_1 *= color_1 * color_4;
          color_2 *= color_2 * color_4;
          color_3 *= color_3 * color_4;
          color_4 *= color_4;

          // Scale the color up to 0..7 . Max brightness is 7.
          //strip.setPixelColor(col + (8 * row), strip.Color(color_4, 0, 0) );

          _matrix.drawPixel(col, row, _matrix.Color(color_1 / 8, color_2 / 8, color_3 / 8));
        }
      }
      _matrix.show();
    }

  protected:
    Adafruit_NeoMatrix _matrix;
    float _phase = 0.0;
};

Screensaver globalSaver(pictureFrame);

#endif
