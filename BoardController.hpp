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

#ifndef _BOARDCONTROLLER_HPP_
#define _BOARDCONTROLLER_HPP_

#include "Accelerometer.hpp"
#include "Levels.hpp"
#include "PictureFrame.hpp"

struct Ball {
  int xInt() {
    return int(x);
  }
  int yInt() {
    return int(y);
  }

  float x = 0.0;
  float y = 0.0;
  float v_x = 0.0;
  float v_y = 0.0;
  const float radius = 0.1;
};

#define EPSILON 0.000001

class BoardController {
  public:
    BoardController(Adafruit_NeoMatrix& matrix, Accelerometer& accel, LevelSet& levels)
      : _matrix(matrix), _accel(accel), _levels(levels),
        _levelIndex(1),
        _lastUpdateTm(0),
        _trapCount(0),
        _palette(_matrix)
    {
    }

    void setLevel(byte index) {
      _levelIndex = index;
      reset();
    }

    uint32_t nextUpdateTm() {
      return _lastUpdateTm + PERIOD;
    }

    void redrawBoard() {
      _matrix.clear();

      for (int i = 0; i < _matrix.width(); ++i) {
        for (int j = 0; j < _matrix.height(); ++j) {
          _matrix.drawPixel(i, j, _palette.colorForCell(level()->cellAt(i, j)));
        }
      }

      _matrix.show();
    }

    void drawBall() {
      _matrix.drawPixel(_ball.xInt(), _ball.yInt(), _palette.ballColor());
      _matrix.show();
    }

    void reset() {
      redrawBoard();

      _ball.v_x = 0.0;
      _ball.v_y = 0.0;

      _ball.x = level()->startX;
      _ball.y = level()->startY;

      drawBall();

      _lastUpdateTm = millis();
    }

    // Returns true if anything on the board changed, false otherwise
    bool update() {
      if (millis() < nextUpdateTm()) {
        return false;
      }

      int startingX = _ball.xInt();
      int startingY = _ball.yInt();

      updateBallPosition();

      updateBallVelocity();

      // If the ball's position has moved across cell boundaries, update the board
      bool changed = false;

      if ((_ball.xInt() != startingX || _ball.yInt() != startingY)) {
        changed = true;

        _matrix.drawPixel(startingX, startingY, _palette.colorForCell(level()->cellAt(startingX, startingY)));
        _matrix.show();

        byte newCell = level()->cellAt(_ball.xInt(), _ball.yInt());

        if (newCell >= Cell_PointerBase) {
          _matrix.drawPixel(startingX, startingY, _palette.colorForCell(level()->cellAt(startingX, startingY)));

          byte destination = newCell - Cell_PointerBase;
          byte row = destination / _matrix.width();
          byte col = destination % _matrix.width();

          animateTeleport(_ball.xInt(), _ball.yInt(), col, row);

          _ball.x = float(col);
          _ball.y = float(row);
          _ball.v_x = 0.0;
          _ball.v_y = 0.0;

          drawBall();

        } else if (newCell >= Cell_WarpBase) {
          animateExit();
          _levelIndex = newCell - Cell_WarpBase;
          reset();

        } else {
          switch (newCell) {
            case Cell_Exit:
              animateExit();
              _levelIndex = max(1, (_levelIndex + 1) % _levels.count); // Prevent exiting to level 0 from the last
              reset();
              break;
            case Cell_Trap:
              // Easter egg - level 0 is accessible from level 1
              if (_levelIndex == 1 && ++_trapCount >= 10) {
                _trapCount = 0;
                animateExit();
                _levelIndex = 0;
                reset();
              } else {
                animateTrap();
                reset();
              }
              break;
            case Cell_Empty: // Move the ball
              drawBall();
          }
        }
      }

      _lastUpdateTm = millis();

      return changed;
    }

    void animateExit() {
      uint32_t innerColor = _palette.colorForCell(Cell_Exit);
      uint32_t outerColor = _palette.ballColor();

      for (int j = 0; j < 6; j++) {
        for (int i = 0; i < 22; i += 2) {
          _matrix.fillCircle(_ball.xInt(), _ball.yInt(), i, outerColor);
          for (int k = i - 1; k >= 0; --k) {
            uint32_t color = (k % 2) == (i % 2) ? outerColor : innerColor;
            _matrix.drawCircle(_ball.xInt(), _ball.yInt(), k, color);
          }
          _matrix.show();
          delay(20);
        }

        uint32_t temp = outerColor;
        outerColor = innerColor;
        innerColor = temp;
      }
    }

    void drawRectCentered(Adafruit_NeoMatrix& gfx, int x, int y, int width, int height, uint16_t color) {
      int leftX = x - width / 2;
      int topY = y - height / 2;
      gfx.drawRect(leftX, topY, width, height, color);
    }

    void animateTrap() {
      uint32_t innerColor = _palette.colorForCell(Cell_Trap);
      uint32_t outerColor = _palette.white;

      for (int j = 0; j < 6; j++) {
        for (int i = 1; i < 8; i += 2) {
          drawRectCentered(_matrix, _ball.xInt(), _ball.yInt(), i, i, outerColor);
          for (int k = i - 1; k >= 0; --k) {
            uint32_t color = (k % 2) == (i % 2) ? outerColor : innerColor;
            drawRectCentered(_matrix, _ball.xInt(), _ball.yInt(), k, k, color);
          }
          _matrix.show();
          delay(20);
        }

        uint32_t temp = outerColor;
        outerColor = innerColor;
        innerColor = temp;
      }
    }

    void animateTeleport(int startX, int startY, int endX, int endY) {
      uint32_t innerColor = _palette.colorForCell(Cell_PointerBase);
      uint32_t outerColor = _palette.ballColor();

      for (int i = 0; i <= 10; i += 2) {
        _matrix.fillCircle(startX, startY, i, outerColor);
        for (int k = i - 1; k >= 0; --k) {
          uint32_t color = (k % 2) == (i % 2) ? outerColor : innerColor;
          _matrix.drawCircle(startX, startY, k, color);
        }
        _matrix.show();
        delay(100);
      }

      redrawBoard();
      delay(100);

      for (int i = 10; i > 0; i -= 2) {
        _matrix.fillCircle(endX, endY, i, innerColor);
        for (int k = i - 1; k >= 0; --k) {
          uint32_t color = (k % 2) == (i % 2) ? innerColor : outerColor;
          _matrix.drawCircle(endX, endY, k, color);
        }
        _matrix.show();
        delay(100);
        redrawBoard();
      }
    }

    struct ColorPalette {
      ColorPalette(Adafruit_NeoMatrix& matrix) {
        orange = matrix.Color(90, 45, 0);
        green = matrix.Color(0, 80, 0);
        white = matrix.Color(60, 60, 60);
        red = matrix.Color(80, 0, 0);
        purple = matrix.Color(90, 0, 90);
        off = matrix.Color(0, 0, 0);
        blue = matrix.Color(0, 0, 80);
      }

      uint32_t ballColor() {
        return blue;
      }

      uint32_t colorForCell(byte cell) {
        if (cell >= Cell_PointerBase) return orange;

        if (cell > Cell_WarpBase) return green;

        switch (cell) {
          case Cell_WarpBase   : return purple;
          case Cell_Wall       : return white;
          case Cell_Wall_Red   : return red;
          case Cell_Trap       : return red;
          case Cell_Wall_Orange: return orange;
          case Cell_Exit       : return green;

          case Cell_Empty      :
          default              : return off;
        }
      }

      uint32_t orange;
      uint32_t green;
      uint32_t white;
      uint32_t red;
      uint32_t purple;
      uint32_t off;
      uint32_t blue;
    };

    bool validLocation(int x, int y) {
      byte potential = level()->cellAt(x, y);
      if (potential >= Cell_WarpBase) { // Covers warps and pointers
        return true;
      } else {
        switch (potential) {
          case Cell_Empty:
          case Cell_Trap:
          case Cell_Exit:
            return true;
          case Cell_Wall:
          case Cell_Wall_Red:
          case Cell_Wall_Orange:
          default:   // Unknown cells treated like walls
            return false;
        }
      }
    }

    void updateBallPosition() {
      // Apply the ball's velocity to its position if possible

      float potentialX = _ball.x + _ball.v_x;
      float potentialY = _ball.y + _ball.v_y;

      if (potentialX < 0) {
        potentialX = 0.0 + _ball.radius;
      } else if (int(potentialX) >= _matrix.width()) {
        potentialX = _matrix.width() - _ball.radius;
      }

      if (potentialY < 0) {
        potentialY = 0.0 + _ball.radius;
      } else if (int(potentialY) >= _matrix.height()) {
        potentialY = _matrix.height() - _ball.radius;
      }

      // If not changing cells, no need to check interference
      if (int(potentialX) == _ball.xInt() && int(potentialY) == _ball.yInt()) {
        _ball.x = potentialX;
        _ball.y = potentialY;
        return;
      }

      if (int(potentialX) != _ball.xInt() && int(potentialY) != _ball.yInt()) {
        // Both X & Y position potentially changing
        byte pathAValid = validLocation(_ball.xInt(), int(potentialY));
        byte pathBValid = validLocation(int(potentialX), _ball.yInt());

        if (validLocation(int(potentialX), int(potentialY))) {
          if (pathAValid && pathBValid) {
            _ball.x = potentialX;
            _ball.y = potentialY;
          } else if (pathAValid) {
            _ball.y = potentialY;
          } else if (pathBValid) {
            _ball.x = potentialX;
          }
        }
      } else if (int(potentialY) != _ball.yInt()) {

        // If potential destination is not a valid location,
        // place the ball as close as it can get.
        if (validLocation(_ball.xInt(), int(potentialY))) {
          _ball.y = potentialY;
        } else if (potentialY > _ball.y) {
          _ball.y = float(int(potentialY)) - _ball.radius;
        } else {
          _ball.y = float(int(potentialY + 1)) + _ball.radius;
        }

        _ball.x = potentialX;
      } else if (int(potentialX) != _ball.xInt()) {

        // If potential destination is not a valid location,
        // place the ball as close as it can get.
        if (validLocation(int(potentialX), _ball.yInt())) {
          _ball.x = potentialX;
        } else if (potentialX > _ball.x) {
          _ball.x = float(int(potentialX)) - _ball.radius;
        } else {
          _ball.x = float(int(potentialX + 1)) + _ball.radius;
        }

        _ball.y = potentialY;
      }
    }

    const float deadZone = 0.08;
    const float velocityDecay = 0.75;

    void updateBallVelocity() {
      float xAccel = -1.0 * _accel.readXAxis();
      if (fabs(xAccel) < deadZone) {
        _ball.v_x *= velocityDecay;
      } else if (xAccel >= 0) {
        // If accelerating against a wall or edge, set velocity to zero
        if (_ball.x + _ball.radius >= float(_matrix.width()) || !validLocation(int(_ball.x + _ball.radius + EPSILON), _ball.yInt())) {
          _ball.v_x = 0.0;
        } else {
          _ball.v_x += xAccel * PERIOD / 1000.0;
        }
      } else {
        if (_ball.x - _ball.radius - EPSILON <= 0.0 || !validLocation(int(_ball.x - _ball.radius - EPSILON), _ball.yInt())) {
          _ball.v_x = 0.0;
        } else {
          _ball.v_x += xAccel * PERIOD / 1000.0;
        }
      }
      _ball.v_x = max(-1.0, min(1.0, _ball.v_x));

      float yAccel = -1.0 * _accel.readYAxis();
      if (fabs(yAccel) < deadZone) {
        _ball.v_y *= velocityDecay;
      } else if (yAccel >= 0) {
        // If accelerating against a wall or edge, set velocity to zero
        if (_ball.y + _ball.radius >= float(_matrix.height()) || !validLocation(_ball.xInt(), int(_ball.y + _ball.radius + EPSILON))) {
          _ball.v_y = 0.0;
        } else {
          _ball.v_y += yAccel * PERIOD / 1000.0;
        }
      } else {
        if (_ball.y - _ball.radius - EPSILON <= 0.0 || !validLocation(_ball.xInt(), int(_ball.y - _ball.radius - EPSILON))) {
          _ball.v_y = 0.0;
        } else {
          _ball.v_y += yAccel * PERIOD / 1000.0;
        }
      }
      _ball.v_y = max(-1.0, min(1.0, _ball.v_y));
    }

  protected:
    Adafruit_NeoMatrix& _matrix;
    Accelerometer& _accel;
    LevelSet& _levels;
    byte _levelIndex;
    long _lastUpdateTm;
    Ball _ball;
    byte _trapCount;
    ColorPalette _palette;

    const long PERIOD = 5; // millis

    Level* level() {
      return &_levels.levels[_levelIndex];
    }
};

BoardController controller(pictureFrame, accelerometer, levels);

#endif
