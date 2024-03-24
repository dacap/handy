// handy text editor
// Copyright (c) 2016-2024 David Capello
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
  return std::string();
}

void View::show(Ctx* ctx) {
  ctx->status()->clear();

  if (mode() == Mode::Sea) {
    ctx->status()->print(("Searching: " + m_searching_text).c_str());
  }
  else {
    ctx->status()->print(get_status_text().c_str());
  }

  ctx->status()->update();
}

bool View::on_key(Ctx* ctx, const Key& key) {
  // Searching text
  if (mode() == Mode::Sea) {
    switch (key.scancode) {
      case Key::Scancode::Enter:
      case Key::Scancode::Escape:
        // Back to previous mode
        if (m_old_modes.empty())
          set_mode(Mode::Nav);
        else {
          Mode restore_mode = m_old_modes.top();
          m_old_modes.pop();
          set_mode(restore_mode);
        }
        return true;
      case Key::Scancode::Backspace: // Backspace remove text
        if (!m_searching_text.empty()) {
          m_searching_text.erase(--m_searching_text.end());
          on_search_text(m_searching_text);
        }
        break;
      default:
        if (key.codepoint != 0) {
          m_searching_text.push_back(key.codepoint);
          on_search_text(m_searching_text);
        }
        break;
    }
  }

  return false;
}

void View::search_text(Ctx* ctx) {
  if (mode() != Mode::Sea) {
    m_old_modes.push(mode());
    set_mode(Mode::Sea);
    m_searching_text.clear();
  }
  else {
    on_search_text(m_searching_text, 1);
  }
}
