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

#ifndef _ACCELEROMETER_HPP_
#define _ACCELEROMETER_HPP_

class Axis {
  public:
    Axis(int pin, int rawMin, int rawMax) {
      this->pin = pin;
      this->rawMin = rawMin;
      this->rawMax = rawMax;
    }

    // Take multiple samples to reduce noise
    const int sampleSize = 10;

    float readAcceleration() {
      long reading = 0;
      analogRead(pin);
      delay(1);
      for (int i = 0; i < sampleSize; i++)
      {
        reading += analogRead(pin);
      }
      int rawReading = reading / sampleSize;
      long scaled = map(rawReading, rawMin, rawMax, -1000, 1000);
      return scaled / 1000.0;
    }

  protected:
    int pin;
    int rawMin;
    int rawMax;
};

class Accelerometer {
  public:
    Accelerometer(Axis& xAxis_, Axis& yAxis_, Axis& zAxis_)
      : xAxis(xAxis_), yAxis(yAxis_), zAxis(zAxis_) {
    }

    float readXAxis() {
      return xAxis.readAcceleration();
    }
    float readYAxis() {
      return yAxis.readAcceleration();
    }
    float readZAxis() {
      return zAxis.readAcceleration();
    }

  protected:
    Axis& xAxis;
    Axis& yAxis;
    Axis& zAxis;
};

const int xInput = A0;
const int yInput = A1;
const int zInput = A2;

// Raw Ranges:
// initialize to mid-range and allow calibration to
// find the minimum and maximum for each axis
int xRawMin = 409;
int xRawMax = 615;

int yRawMin = 404;
int yRawMax = 612;

int zRawMin = 433;
int zRawMax = 640;

Axis xAxis(xInput, xRawMin, xRawMax);
Axis yAxis(yInput, yRawMin, yRawMax);
Axis zAxis(zInput, zRawMin, zRawMax);

Accelerometer accelerometer(xAxis, yAxis, zAxis);

#endif
