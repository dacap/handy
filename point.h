// handy text editor
// Copyright (c) 2016-2018 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

struct Point {
  int x, y;                     // x=column, y=row
  Point() : x(0), y(0) {}
  Point(int x, int y) : x(x), y(y) { }
  bool operator==(const Point& p) {
    return x == p.x && y == p.y;
  }
};
