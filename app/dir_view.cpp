// handy text editor
// Copyright (c) 2016-2024 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "dir_view.h"

#include "fmt/format.h"
#include "git_view.h"

DirView::DirView() {
  open_path(base::get_current_path());
}

std::string DirView::get_status_text() const {
  return fmt::format("-- {} --", m_path);
}

void DirView::show(Ctx* ctx) {
  PanelPtr panel = this->panel();
  panel->clear();
  panel->move(0, 0);

  const int last_visible = std::min<int>(m_scroll.y+panel->height(),
                                         m_files.size());

  for (int i=m_scroll.y; i<last_visible; ++i) {
    const std::string& f = m_files[i];

    if (m_selected == i)
      panel->attr_reverse();
    else
      panel->attr_normal();

    panel->print((f+"\n").c_str());
  }

  panel->update();
  View::show(ctx);
}

bool DirView::on_key(Ctx* ctx, const Key& key) {
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
      if (m_selected > 0) {
        --m_selected;
        update_scroll();
      }
      break;
    case Key::Scancode::KeyK:
    case Key::Scancode::ArrowDown:
      if (m_selected < int(m_files.size()-1)) {
        ++m_selected;
        update_scroll();
      }
      break;
      // Git Status
    case Key::Scancode::KeyS:
      if (mode() == Mode::Nav) {
        ctx->set_view(std::make_shared<GitView>(m_path));
        return true;
      }
      break;
    case Key::Scancode::Enter:  // Enter open the selected file
      // open file
      if (m_selected >= 0 && m_selected < int(m_files.size())) {
        std::string fn = base::join_path(m_path, m_files[m_selected]);
        fn = base::normalize_path(fn);
        if (base::is_file(fn))
          ctx->open_file(fn.c_str());
        else if (base::is_directory(fn))
          open_path(fn);
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

void DirView::on_search_text(const std::string& text, int skip)
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
        update_scroll();
        break;
      }
    }

    ++i;
  }
}

void DirView::open_path(const std::string& path)
{
  m_path = path;

  m_files.clear();
  m_files.push_back(".");
  m_files.push_back("..");

  auto list = base::list_files(m_path);
  m_files.insert(m_files.end(), list.begin(), list.end());

  m_selected = 0;
  m_scroll = Point(0, 0);
}

void DirView::update_scroll() {
  // Update scroll
  PanelPtr panel = this->panel();
  Point scroll = m_scroll;
  if (m_selected < scroll.y) scroll.y = m_selected;
  if (m_selected > scroll.y+panel->height()-1) scroll.y = m_selected-panel->height()+1;
  m_scroll = scroll;
}
