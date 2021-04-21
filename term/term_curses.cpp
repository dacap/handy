// handy text editor
// Copyright (c) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include <curses.h>

typedef WINDOW PanelImpl;
struct TermImpl { };
#define TERM_IMPL
#include "term.h"

Panel::Panel(int x, int y, int w, int h) {
  m_impl = newwin(h, w, y, x);
  raw();
  keypad(m_impl, TRUE);
  meta(m_impl, TRUE);
}

Panel::~Panel() {
  delwin(m_impl);
}

int Panel::x() {
  int y, x;
  getbegyx(m_impl, y, x);
  return x;
}

int Panel::y() {
  int y, x;
  getbegyx(m_impl, y, x);
  return y;
}

int Panel::width() {
  int y, x;
  getmaxyx(m_impl, y, x);
  return x;
}

int Panel::height() {
  int y, x;
  getmaxyx(m_impl, y, x);
  return y;
}

void Panel::clear() {
  wclear(m_impl);
}

void Panel::print(const char* s) {
  wprintw(m_impl, s);
}

void Panel::print(const char ch) {
  waddch(m_impl, ch);
}

void Panel::move(int x, int y) {
  wmove(m_impl, y, x);
}

void Panel::update() {
  wrefresh(m_impl);
}

void Panel::update_lines(int y, int h) {
  wredrawln(m_impl, y, h);
}

int Panel::get_char() {
  return wgetch(m_impl);
}

void Panel::attr_reverse()
{
  wattron(m_impl, A_REVERSE);
}

void Panel::attr_normal()
{
  wattroff(m_impl, A_REVERSE);
}

Term::Term() : m_impl(nullptr) {
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
