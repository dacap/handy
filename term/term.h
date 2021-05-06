// handy text editor
// Copyright (c) 2016-2021 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "fwd.h"

class Panel {
public:
  virtual ~Panel() { }
  virtual int x() const = 0;
  virtual int y() const = 0;
  virtual int width() const = 0;
  virtual int height() const = 0;
  virtual void clear() = 0;
  virtual void print(const char* s) = 0;
  virtual void print(const char ch) = 0;
  virtual void move(int x, int y) = 0;
  virtual void update() = 0;
  virtual void update_lines(int y, int h) = 0;
  virtual int get_char() = 0;
  virtual void attr_reverse() = 0;
  virtual void attr_normal() = 0;
};

class Term {
public:
  static TermPtr MakeTUI();
  static TermPtr MakeGUI();

  virtual ~Term() { }
  virtual int width() const = 0;
  virtual int height() const = 0;
  virtual PanelPtr makePanel(int x, int y, int w, int h) = 0;
};
