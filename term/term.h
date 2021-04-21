// handy text editor
// Copyright (c) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#ifndef TERM_IMPL
struct PanelImpl;
struct TermImpl;
#define KEY_ESC   27
#define KEY_DOWN  258
#define KEY_UP    259
#define KEY_LEFT  260
#define KEY_RIGHT 261
#endif

class Panel {
public:
  Panel(int x, int y, int w, int h);
  ~Panel();
  int x();
  int y();
  int width();
  int height();
  void clear();
  void print(const char* s);
  void print(const char ch);
  void move(int x, int y);
  void update();
  void update_lines(int y, int h);
  int get_char();
  void attr_reverse();
  void attr_normal();
private:
  PanelImpl* m_impl;
};

class Term {
public:
  Term();
  ~Term();
  int width();
  int height();
private:
  TermImpl* m_impl;
};
