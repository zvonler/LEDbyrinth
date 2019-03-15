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

#ifndef _LEVELS_HPP_
#define _LEVELS_HPP_

const byte Cell_Empty = 0;
const byte Cell_Wall = 1;
const byte Cell_Trap = 2;
const byte Cell_Exit = 3;
const byte Cell_Wall_Purple = 4;
const byte Cell_Wall_Cyan = 5;

const byte Cell_WarpBase = 20;
// Cell values from WarpBase to PointerBase are used to warp to levels.
const byte Cell_PointerBase = 35;
// Cell values from PointerBase up are used as pointers for teleports within a level.

// Implements a Compressed Sparse Row matrix of byte values.
class Level {
  public:
    Level(byte startX_, byte startY_, const byte *rowIndices, const byte *columnIndices, const byte *values)
      : startX(startX_), startY(startY_), _rowIndices(rowIndices), _columnIndices(columnIndices), _values(values)
    { }

    byte cellAt(int x, int y) {
      // Row y's column indices start at columnIndices[rowIndices[y]] and
      // its values start at values[rowIndices[y]]
      byte rowStartIndex = pgm_read_byte_near(_rowIndices + y);
      byte rowEndIndex = pgm_read_byte_near(_rowIndices + y + 1);

      for (int i = rowStartIndex; i < rowEndIndex; ++i) {
        if (pgm_read_byte_near(_columnIndices + i) == x) {
          return pgm_read_byte_near(_values + i);
        }
      }

      return Cell_Empty;
    }

    Level* next = 0;

    const byte startX;
    const byte startY;

  protected:
    const byte* _rowIndices;
    const byte* _columnIndices;
    const byte* _values;
};

const byte level1_rowIndices[] PROGMEM = { 0, 3, 5, 7, 10, 14, 18, 22, 26, 29, 37, 37, 37, 37 };
const byte level1_columnIndices[] PROGMEM = {
  7, 8, 9, 7, 9, 1, 14, 1, 13, 14,
  1, 7, 9, 14, 1, 7, 9, 14, 1, 7, 9, 14, 1, 7, 9, 14,
  1, 8, 14, 1, 2, 3, 4, 8, 13, 14, 15
};
const byte level1_values[] PROGMEM = {
  Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Exit, Cell_Wall,
  Cell_Wall, Cell_Wall
};

const byte level2_rowIndices[] PROGMEM = { 0, 4, 6, 6, 8, 8, 8, 8, 8, 8, 10, 10, 12, 16 };
const byte level2_columnIndices[] PROGMEM = { 0, 3, 13, 16, 1, 15, 0, 16, 0, 16, 1, 15, 0, 3, 13, 16 };
const byte level2_values[] PROGMEM = {
  Cell_Trap, Cell_Wall, Cell_Wall, Cell_PointerBase + 204, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_PointerBase + 16,
  Cell_Wall, Cell_Wall, Cell_Exit
};

const byte puzzleLevel1_rowIndices[] PROGMEM = { 0, 7, 16, 20, 27, 34, 43, 51, 58, 68, 76, 83, 90, 98 };
const byte puzzleLevel1_columnIndices[] PROGMEM = {
  0, 6, 7, 8, 9, 13, 16,
  0, 2, 3, 4, 7, 9, 11, 13, 16,
  4, 11, 14, 16,
  1, 2, 4, 6, 7, 9, 12,
  2, 4, 7, 9, 10, 13, 15,
  0, 1, 4, 5, 7, 9, 11, 13, 15,
  0, 1, 3, 7, 9, 11, 13, 16,
  3, 5, 6, 7, 8, 9, 14,
  0, 2, 5, 8, 9, 10, 11, 12, 14, 15,
  0, 2, 4, 5, 7, 8, 12, 15,
  2, 5, 10, 12, 13, 14, 15,
  1, 4, 5, 7, 8, 10, 14,
  0, 1, 3, 4, 7, 11, 12, 16
};
const byte puzzleLevel1_values[] PROGMEM = {
  Cell_Trap, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Exit, Cell_Wall,
  Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_PointerBase + 208, Cell_Wall, Cell_PointerBase + 204, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Trap
};

const byte puzzleLevel2_rowIndices[] PROGMEM = { 0, 9, 16, 19, 32, 37, 51, 55, 66, 72, 79, 90, 96, 109 };
const byte puzzleLevel2_columnIndices[] PROGMEM = {
  0, 6, 7, 8, 9, 10, 11, 15, 16,
  2, 3, 4, 6, 11, 13, 15,
  4, 8, 13,
  0, 1, 2, 4, 6, 8, 10, 11, 12, 13, 14, 15, 16,
  4, 8, 9, 15, 16,
  1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 15, 16,
  5, 9, 11, 12,
  0, 1, 2, 4, 5, 7, 8, 9, 11, 14, 16,
  0, 4, 5, 9, 11, 13,
  0, 6, 7, 9, 11, 13, 15,
  0, 2, 3, 4, 7, 9, 11, 13, 14, 15, 16,
  0, 2, 3, 4, 7, 11,
  0, 2, 3, 4, 5, 7, 8, 9, 10, 13, 14, 15, 16
};
const byte puzzleLevel2_values[] PROGMEM = {
  Cell_Trap, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Exit, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Trap, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall,
  Cell_Wall, Cell_Trap, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall,
  Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_PointerBase + 220, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Trap, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_PointerBase + 204
};

const byte easySpiral_rowIndices[] PROGMEM = { 0, 10, 20, 34, 40, 52, 61, 72, 81, 93, 99, 113, 123, 132 };
const byte easySpiral_columnIndices[] PROGMEM = {
  0, 1, 2, 3, 11, 12, 13, 14, 15, 16,
  0, 1, 5, 6, 7, 8, 9, 13, 14, 15,
  0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15,
  0, 1, 3, 11, 13, 14,
  0, 1, 3, 5, 6, 7, 8, 9, 11, 13, 14, 16,
  0, 3, 5, 8, 9, 11, 13, 14, 16,
  0, 2, 3, 5, 7, 8, 9, 11, 13, 14, 16,
  0, 2, 3, 5, 7, 8, 11, 13, 16,
  0, 2, 3, 5, 7, 8, 9, 10, 11, 13, 15, 16,
  2, 3, 5, 13, 15, 16,
  1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16,
  1, 2, 3, 7, 8, 9, 10, 11, 15, 16,
  1, 2, 3, 4, 5, 13, 14, 15, 16
};
const byte easySpiral_values[] PROGMEM = {
  Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Exit, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall,
  Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Trap,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_PointerBase + 127, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_PointerBase + 93, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall
};

const byte hardSpiral_rowIndices[] PROGMEM = { 0, 10, 20, 34, 40, 52, 60, 71, 80, 92, 98, 112, 122, 132 };
const byte hardSpiral_columnIndices[] PROGMEM = {
  0, 1, 2, 3, 11, 12, 13, 14, 15, 16,
  0, 1, 5, 6, 7, 8, 9, 13, 14, 15,
  0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 15,
  0, 1, 3, 11, 13, 14,
  0, 1, 3, 5, 6, 7, 8, 9, 11, 13, 14, 16,
  0, 3, 5, 9, 11, 13, 14, 16,
  0, 2, 3, 5, 7, 8, 9, 11, 13, 14, 16,
  0, 2, 3, 5, 7, 8, 11, 13, 16,
  0, 2, 3, 5, 7, 8, 9, 10, 11, 13, 15, 16,
  2, 3, 5, 13, 15, 16,
  1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16,
  1, 2, 3, 7, 8, 9, 10, 11, 15, 16,
  0, 1, 2, 3, 4, 5, 13, 14, 15, 16
};
const byte hardSpiral_values[] PROGMEM = {
  Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_PointerBase + 204, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap,
  Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall,
  Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Exit, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall,
  Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_PointerBase + 16, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall
};

const byte bigXlevel_rowIndices[] PROGMEM = { 0, 3, 7, 10, 14, 18, 22, 32, 36, 40, 44, 47, 51, 54 };
const byte bigXlevel_columnIndices[] PROGMEM = {
  0, 8, 16,
  3, 7, 9, 13,
  4, 8, 12,
  1, 5, 11, 15,
  2, 6, 10, 14,
  3, 7, 9, 13,
  0, 1, 3, 4, 8, 9, 12, 13, 15, 16,
  3, 7, 9, 13,
  2, 6, 10, 14,
  1, 5, 11, 15,
  4, 8, 12,
  3, 7, 9, 13,
  0, 8, 16
};
const byte bigXlevel_values[] PROGMEM = {
  Cell_Trap, Cell_Trap, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall,
  Cell_PointerBase + 115, Cell_Wall, Cell_Trap, Cell_Exit, Cell_Wall, Cell_PointerBase + 105,
  Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Trap, Cell_Trap
};

const byte effigyLevel_rowIndices[] PROGMEM = { 0, 0, 3, 7, 15, 20, 23, 29, 32, 37, 45, 49, 52, 52 };
const byte effigyLevel_columnIndices[] PROGMEM = {
  1, 8, 15,
  2, 3, 13, 14,
  3, 4, 5, 6, 10, 11, 12, 13,
  6, 7, 8, 9, 10,
  8, 14, 15,
  1, 8, 11, 13, 14, 15,
  8, 14, 15,
  6, 7, 8, 9, 10,
  3, 4, 5, 6, 10, 11, 12, 13,
  2, 3, 13, 14,
  1, 8, 15
};
const byte effigyLevel_values[] PROGMEM = {
  Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_PointerBase + 144, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Trap, Cell_Trap, Cell_Exit, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_PointerBase + 76, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall
};

const byte level0_rowIndices[] PROGMEM = { 0, 8, 8, 12, 12, 12, 12, 16, 16, 16, 16, 20, 20, 31 };
const byte level0_columnIndices[] PROGMEM = {
  2, 5, 6, 9, 10, 11, 14, 15,
  2, 6, 10, 14,
  0, 2, 14, 16,
  2, 6, 10, 14,
  1, 2, 4, 5, 6, 7, 9, 10, 11, 14, 15
};
const byte level0_values[] PROGMEM = {
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall_Cyan, // V
  Cell_WarpBase + 1, Cell_WarpBase + 2, Cell_WarpBase + 3, Cell_WarpBase + 4,
  Cell_Wall_Purple, // X
  Cell_WarpBase + 10, Cell_WarpBase + 5,
  Cell_Wall_Cyan, // V
  Cell_WarpBase + 9, Cell_WarpBase + 8, Cell_WarpBase + 7, Cell_WarpBase + 6,
  Cell_Wall,
  Cell_Wall_Purple, // X
  Cell_Wall_Cyan, // V
  Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall_Cyan, // V
  Cell_Wall, Cell_Wall,
  Cell_Wall_Cyan, // V
  Cell_Wall
};

const byte multiWarpLevel_rowIndices[] PROGMEM = { 0, 7, 18, 24, 34, 39, 48, 55, 66, 72, 81, 87, 97, 104 };
const byte multiWarpLevel_columnIndices[] PROGMEM = {
  0, 1, 2, 3, 4, 9, 16,
  1, 2, 6, 7, 8, 9, 11, 12, 13, 15, 16,
  4, 5, 11, 13, 15, 16,
  0, 2, 4, 7, 8, 9, 10, 13, 15, 16,
  2, 3, 7, 11, 13,
  1, 2, 5, 7, 9, 11, 13, 14, 15,
  4, 5, 7, 8, 9, 11, 13,
  0, 1, 2, 3, 5, 8, 9, 11, 13, 15, 16,
  0, 3, 5, 6, 8, 11,
  0, 2, 3, 7, 10, 11, 12, 13, 15,
  0, 5, 7, 9, 13, 15,
  0, 1, 2, 3, 5, 9, 10, 11, 13, 15,
  0, 5, 6, 7, 11, 15, 16
};
const byte multiWarpLevel_values[] PROGMEM = {
  Cell_PointerBase + 16, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_PointerBase + 220,
  Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap,
  Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Trap, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Exit,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Trap, Cell_Trap, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Trap, Cell_Trap, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall,
  Cell_PointerBase, Cell_Wall, Cell_Wall, Cell_Trap, Cell_Wall, Cell_Wall, Cell_PointerBase + 204
};

const byte finalLevel_rowIndices[] PROGMEM = { 0, 4, 9, 13, 29, 36, 36, 43, 59, 75, 75, 76, 76, 93 };
const byte finalLevel_columnIndices[] PROGMEM = {
  1, 5, 9, 13,
  3, 7, 11, 15, 16,
  1, 5, 9, 13,
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
  2, 4, 6, 8, 10, 12, 14,
  2, 4, 6, 8, 10, 12, 14,
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
  8,
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
};
const byte finalLevel_values[] PROGMEM = {
  Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Exit,
  Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap,
  Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap,
  Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap,
  Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap,
  Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap,
  Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap,
  Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap,
  Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap,
  Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap, Cell_Trap,
  Cell_Trap, Cell_Trap, Cell_Trap
};

const byte winnerLevel_rowIndices[] PROGMEM = { 0, 0, 0, 6, 12, 17, 22, 22, 28, 35, 43, 50, 50, 51 };
const byte winnerLevel_columnIndices[] PROGMEM = {
  1, 5, 8, 9, 12, 15,
  2, 4, 7, 10, 12, 15,
  3, 7, 10, 12, 15,
  3, 8, 9, 13, 14,
  1, 5, 8, 9, 12, 15,
  1, 5, 7, 10, 12, 13, 15,
  1, 3, 5, 7, 10, 12, 14, 15,
  2, 3, 4, 8, 9, 12, 15,
  16
};
const byte winnerLevel_values[] PROGMEM = {
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_WarpBase, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall,
  Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Wall, Cell_Exit
};

struct LevelSet {
  Level *levels;
  byte count;

  LevelSet(Level* levels_, int count_)
    : levels(levels_), count(count_)
  { }
};

Level levelsArr[] = {
  Level(8, 6, level0_rowIndices, level0_columnIndices, level0_values),
  Level(8, 6, level1_rowIndices, level1_columnIndices, level1_values),
  Level(8, 6, level2_rowIndices, level2_columnIndices, level2_values),
  Level(4, 6, effigyLevel_rowIndices, effigyLevel_columnIndices, effigyLevel_values),
  Level(0, 12, easySpiral_rowIndices, easySpiral_columnIndices, easySpiral_values),
  Level(8, 6, puzzleLevel1_rowIndices, puzzleLevel1_columnIndices, puzzleLevel1_values),
  Level(7, 6, bigXlevel_rowIndices, bigXlevel_columnIndices, bigXlevel_values),
  Level(8, 5, hardSpiral_rowIndices, hardSpiral_columnIndices, hardSpiral_values),
  Level(8, 6, puzzleLevel2_rowIndices, puzzleLevel2_columnIndices, puzzleLevel2_values),
  Level(8, 0, multiWarpLevel_rowIndices, multiWarpLevel_columnIndices, multiWarpLevel_values),
  Level(0, 10, finalLevel_rowIndices, finalLevel_columnIndices, finalLevel_values),
  Level(3, 3, winnerLevel_rowIndices, winnerLevel_columnIndices, winnerLevel_values),
};

LevelSet levels(levelsArr, 12);

#endif
