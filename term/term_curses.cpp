// handy text editor
// Copyright (c) 2016-2021 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include <curses.h>

#include "term.h"

class PanelCurses : public Panel {
  WINDOW* m_win;
public:

  PanelCurses(int x, int y, int w, int h) {
    m_win = newwin(h, w, y, x);
    raw();
    keypad(m_win, TRUE);
    meta(m_win, TRUE);
  }

  ~PanelCurses() override {
    delwin(m_win);
  }

  int x() const override {
    int y, x;
    getbegyx(m_win, y, x);
    return x;
  }

  int y() const override {
    int y, x;
    getbegyx(m_win, y, x);
    return y;
  }

  int width() const override {
    int y, x;
    getmaxyx(m_win, y, x);
    return x;
  }

  int height() const override {
    int y, x;
    getmaxyx(m_win, y, x);
    return y;
  }

  void clear() override {
    wclear(m_win);
  }

  void print(const char* s) override {
    wprintw(m_win, s);
  }

  void print(const char ch) override {
    waddch(m_win, ch);
  }

  void move(int x, int y) override {
    wmove(m_win, y, x);
  }

  void update() override {
    wrefresh(m_win);
  }

  void update_lines(int y, int h) override {
    wredrawln(m_win, y, h);
  }

  int get_char() override {
    return wgetch(m_win);
  }

  void attr_reverse() override {
    wattron(m_win, A_REVERSE);
  }

  void attr_normal() override {
    wattroff(m_win, A_REVERSE);
  }

};

class TermCurses : public Term {
public:
  TermCurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
  }

  ~TermCurses() override {
    endwin();
  }

  int width() const override {
    int y, x;
    getmaxyx(stdscr, y, x);
    return x;
  }

  int height() const override {
    int y, x;
    getmaxyx(stdscr, y, x);
    return y;
  }

  PanelPtr makePanel(int x, int y, int w, int h) override {
    return std::make_shared<PanelCurses>(x, y, w, h);
  }

};

// static
TermPtr Term::MakeTUI() {
  return std::make_shared<TermCurses>();
}
