// handy text editor
// Copyright (c) 2016-2017 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "view.h"

View::View()
  : m_mode(Mode::Nav) {
}

View::~View() {
}

std::string View::get_status_text() const {
  return "";
}

void View::show(Ctx* ctx) {
  ctx->status()->clear();

  if (mode() == Mode::Sea) {
    ctx->status()->print(("Searching: " + m_searchingText).c_str());
  }
  else {
    ctx->status()->print(get_status_text().c_str());
  }

  ctx->status()->update();
}

bool View::on_key(Ctx* ctx, int ch) {
  // Searching text
  if (mode() == Mode::Sea) {
    switch (ch) {
      case 27:
        set_mode(Mode::Nav);  // Back to navigation
        return true;
      default:
        m_searchingText.push_back(ch);
        break;
    }
  }

  return false;
}

void View::search_text(Ctx* ctx) {
  set_mode(Mode::Sea);
  m_searchingText = "";
}
