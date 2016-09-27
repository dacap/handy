// handy text editor
// Copyright (c) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

struct Pos {
  int x, y;
  Pos() : x(0), y(0) {}
  Pos(int x, int y) : x(x), y(y) { }
  bool operator==(const Pos& p) {
    return x == p.x && y == p.y;
  }
};
