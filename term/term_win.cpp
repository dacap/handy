// handy text editor
// Copyright (c) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include <windows.h>

struct PanelImpl;
struct TermImpl;
#define TERM_IMPL
#include "term.h"

struct PanelImpl {
  int x, y, w, h;

  PanelImpl(int x, int y, int w, int h)
    : x(x), y(y), w(w), h(h) {
  }
};

struct TermImpl {
  HANDLE hstdout;

  TermImpl() {
    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
  }

  int width() {
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(hstdout, &info))
      return info.srWindow.Right - info.srWindow.Left;
    else
      return 80;
  }

  int height() {
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(hstdout, &info))
      return info.srWindow.Bottom - info.srWindow.Top;
    else
      return 25;
  }
};

Panel::Panel(int x, int y, int w, int h)
  : m_impl(new PanelImpl(x, y, w, h)) {
}

Panel::~Panel() {
  delete m_impl;
}

int Panel::x() {
  return m_impl->x;
}

int Panel::y() {
  return m_impl->y;
}

int Panel::width() {
  return m_impl->w;
}

int Panel::height() {
  return m_impl->h;
}

void Panel::clear() {
  // TODO
}

void Panel::print(const char* s) {
  // TODO
}

void Panel::print(const char ch) {
  // TODO
}

void Panel::move(int x, int y) {
  // TODO
}

void Panel::update() {
  // TODO
}

void Panel::update_lines(int y, int h) {
  // TODO
}

int Panel::get_char() {
  // TODO
  return 0;
}

void Panel::attr_reverse()
{
  // TODO
}

void Panel::attr_normal()
{
  // TODO
}

Term::Term() : m_impl(new TermImpl) {
}

Term::~Term() {
  delete m_impl;
}

int Term::width() {
  return m_impl->width();
}

int Term::height() {
  return m_impl->height();
}
