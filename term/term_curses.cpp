// handy text editor
// Copyright (c) 2016-2024 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include <curses.h>

#include "event.h"
#include "term.h"

static Key key_from_curses_char(int ch) {
  Key key;

  switch (ch) {

    case 10:
      key.scancode = Key::Scancode::Enter;
      break;

    case 27:
      key.scancode = Key::Scancode::Escape;
      break;

    case 32:
      key.scancode = Key::Scancode::Space;
      key.codepoint = ' ';
      break;

    case KEY_DOWN:      key.scancode = Key::Scancode::ArrowDown;  break;
    case KEY_UP:        key.scancode = Key::Scancode::ArrowUp;    break;
    case KEY_LEFT:      key.scancode = Key::Scancode::ArrowLeft;  break;
    case KEY_RIGHT:     key.scancode = Key::Scancode::ArrowRight; break;

    case 127:
    case KEY_BACKSPACE:
      key.scancode = Key::Scancode::Backspace;
      break;

    case KEY_PPAGE:     key.scancode = Key::Scancode::PageUp;     break;
    case KEY_NPAGE:     key.scancode = Key::Scancode::PageDown;   break;

    case KEY_SHOME:
      key.modifiers = Key::Modifiers::Shift;
      [[fallthrough]];
    case KEY_HOME:
      key.scancode = Key::Scancode::Home;
      break;

    case KEY_SEND:
      key.modifiers = Key::Modifiers::Shift;
      [[fallthrough]];
    case KEY_END:
      key.scancode = Key::Scancode::End;
      break;

    default:
      // Ctrl+A ... Ctrl+Z
      if (ch >= 1 && ch < 27) {
        key.scancode = Key::Scancode(int(Key::Scancode::KeyA) + ch - 1);
        key.modifiers = Key::Modifiers::Ctrl;
      }
      // A ... Z
      else if (ch >= 'A' && ch <= 'Z') {
        key.scancode = Key::Scancode(int(Key::Scancode::KeyA) + ch - 'A');
        key.modifiers = Key::Modifiers::Shift;
        key.codepoint = ch;
      }
      // a ... z
      else if (ch >= 'a' && ch <= 'z') {
        key.scancode = Key::Scancode(int(Key::Scancode::KeyA) + ch - 'a');
        key.codepoint = ch;
      }
      // 0 ... 9
      else if (ch >= '0' && ch <= '9') {
        key.scancode = Key::Scancode(int(Key::Scancode::Digit0) + ch - '0');
        key.codepoint = ch;
      }
      break;

  }
  return key;
}

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

  Event get_event() override {
    Event ev(Event::Type::Key);

    // Get key from ncurses
    int ch = wgetch(m_win);
    Key key = key_from_curses_char(ch);

    ev.key(key);
    return ev;
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
