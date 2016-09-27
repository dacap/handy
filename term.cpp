// handy text editor
// Copyright (c) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include <curses.h>
#define TERM_IMPL
#include "term.h"

Panel::Panel(int x, int y, int w, int h) {
  m_win = newwin(h, w, y, x);
  raw();
  keypad(m_win, TRUE);
  meta(m_win, TRUE);
}

Panel::~Panel() {
  delwin(m_win);
}

int Panel::x() {
  int y, x;
  getbegyx(m_win, y, x);
  return x;
}

int Panel::y() {
  int y, x;
  getbegyx(m_win, y, x);
  return y;
}

int Panel::width() {
  int y, x;
  getmaxyx(m_win, y, x);
  return x;
}

int Panel::height() {
  int y, x;
  getmaxyx(m_win, y, x);
  return y;
}

void Panel::clear() {
  wclear(m_win);
}

void Panel::print(const char* s) {
  wprintw(m_win, s);
}

void Panel::print(const char ch) {
  waddch(m_win, ch);
}

void Panel::move(int x, int y) {
  wmove(m_win, y, x);
}

void Panel::update() {
  wrefresh(m_win);
}

void Panel::update_lines(int y, int h) {
  wredrawln(m_win, y, h);
}

int Panel::get_char() {
  return wgetch(m_win);
}

void Panel::attr_reverse()
{
  wattron(m_win, A_REVERSE);
}

void Panel::attr_normal()
{
  wattroff(m_win, A_REVERSE);
}

Term::Term() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
}

Term::~Term() {
  endwin();
}

int Term::width() {
  int y, x;
  getmaxyx(stdscr, y, x);
  return x;
}

int Term::height() {
  int y, x;
  getmaxyx(stdscr, y, x);
  return y;
}
