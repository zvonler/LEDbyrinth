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

#ifndef _ORIENTATION_HPP_
#define _ORIENTATION_HPP_

class Orientation {
  public:
    Orientation(float changeThreshold)
      : _threshold(changeThreshold)
      , _initialX(accelerometer.readXAxis())
      , _initialY(accelerometer.readYAxis())
      , _initialZ(accelerometer.readZAxis())
    {
    }

    bool changed() {
      return fabs(accelerometer.readXAxis() - _initialX) > _threshold ||
             fabs(accelerometer.readYAxis() - _initialY) > _threshold ||
             fabs(accelerometer.readZAxis() - _initialZ) > _threshold;
    }

  private:
    float _threshold;
    float _initialX;
    float _initialY;
    float _initialZ;
};

#endif
