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

Adafruit_NeoMatrix pictureFrame = Adafruit_NeoMatrix(
                                    17, // width
                                    13, // height
                                    6,  // pin
                                    NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
                                    NEO_MATRIX_ROWS    + NEO_MATRIX_ZIGZAG,
                                    NEO_GRB            + NEO_KHZ800);
#endif
