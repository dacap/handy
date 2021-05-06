// handy text editor
// Copyright (c) 2016-2021 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include <windows.h>

#include "term.h"

class PanelWin : public Panel {
  int m_x, m_y, m_w, m_h;

public:
  PanelWin(int x, int y, int w, int h)
    : m_x(x), m_y(y), m_w(w), m_h(h) {
  }

  int x() const override {
    return m_x;
  }

  int y() const override {
    return m_y;
  }

  int width() const override {
    return m_w;
  }

  int height() const override {
    return m_h;
  }

  void clear() {
    // TODO
  }

  void print(const char* s) {
    // TODO
  }

  void print(const char ch) {
    // TODO
  }

  void move(int x, int y) {
    // TODO
  }

  void update() {
    // TODO
  }

  void update_lines(int y, int h) {
    // TODO
  }

  int get_char() {
    // TODO
    return 0;
  }

  void attr_reverse() {
    // TODO
  }

  void attr_normal() {
    // TODO
  }

};

class TermWin : public Term {
  HANDLE hstdout;

public:
  TermWin() {
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
  }

  int width() const override {
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(hstdout, &info))
      return info.srWindow.Right - info.srWindow.Left;
    else
      return 80;
  }

  int height() const override {
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(hstdout, &info))
      return info.srWindow.Bottom - info.srWindow.Top;
    else
      return 25;
  }

  PanelPtr makePanel(int x, int y, int w, int h) override {
    return std::make_shared<PanelWin>(x, y, w, h);
  }

};

// static
TermPtr Term::MakeTUI() {
  return std::make_shared<TermWin>();
}
