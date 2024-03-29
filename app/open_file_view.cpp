// handy text editor
// Copyright (c) 2016-2024 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "open_file_view.h"

OpenFileView::OpenFileView() {
  m_path = base::get_current_path();
  m_files = base::list_files(m_path);
  m_selected = 0;
}

std::string OpenFileView::get_status_text() const {
  return "-- " + m_path + " --";
}

void OpenFileView::show(Ctx* ctx) {
  PanelPtr panel = this->panel();
  panel->clear();
  panel->move(0, 0);

  int i = 0;
  for (const auto& f : m_files) {
    if (m_selected == i)
      panel->attr_reverse();
    else
      panel->attr_normal();

    panel->print((f+"\n").c_str());
    ++i;
  }

  panel->update();
  View::show(ctx);
}

bool OpenFileView::on_key(Ctx* ctx, const Key& key) {
  switch (key.scancode) {
    case Key::Scancode::KeyQ:
    case Key::Scancode::Escape:
      if (mode() == Mode::Nav) {
        ctx->back_view();
        return true;
      }
      break;
    case Key::Scancode::KeyI:
    case Key::Scancode::ArrowUp:
      if (m_selected > 0)
        --m_selected;
      break;
    case Key::Scancode::KeyK:
    case Key::Scancode::ArrowDown:
      if (m_selected < int(m_files.size()-1))
        ++m_selected;
      break;
    case Key::Scancode::Enter:  // Enter open the selected file
      // open file
      if (m_selected >= 0 && m_selected < int(m_files.size())) {
        ctx->open_file(
          base::join_path(m_path, m_files[m_selected]).c_str());
      }
      break;
    case Key::Scancode::KeyF:
      // Ctrl+F
      if (key.ctrlKey()) {
        search_text(ctx);
        return true;
      }
      break;
    case Key::Scancode::KeyY:
      search_text(ctx);
      return true;
  }
  return View::on_key(ctx, key);
}

void OpenFileView::on_search_text(const std::string& text, int skip)
{
  bool continue_search = false;
  int i = 0;
  for (const auto& f : m_files) {
    if (m_selected == i)
      continue_search = true;

    if (continue_search &&
        f.find(text) != std::string::npos) {
      if (skip-- == 0) {
        m_selected = i;
        break;
      }
    }

    ++i;
  }
}
