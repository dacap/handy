// handy text editor
// Copyright (c) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include <curses.h>
#include <stdio.h>

// This is a temporary tool to get key codes
int main() {
  initscr();
  cbreak();
  noecho();
  raw();
  keypad(stdscr, TRUE);
  meta(stdscr, TRUE);
  int i = 0;

  int h, w;
  getmaxyx(stdscr, h, w);

  while (true) {
    int ch = getch();

    if (((i++) % h) == 0)
      clear();
    printw("KEY NAME : %s - %d\n", keyname(ch), ch);

    if (ch == 3) {              // Ctrl+C stops the loop
      break;
    }
  }
  endwin();
}
