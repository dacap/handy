// handy text editor
// Copyright (c) 2024 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "git_view.h"

#include "base/fs.h"
#include "base/trim_string.h"

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <vector>

#include "fmt/format.h"

#if WIN32
#define popen _popen
#define pclose _pclose
#endif

void GitView::Item::fill_hunks()
{
  hunks.clear();

  auto pipe = popen(("git diff " + file).c_str(), "r");
  if (pipe) {
    std::vector<char> buf(1024);
    Hunk hunk;
    while (fgets(buf.data(), buf.size(), pipe)) {
      hunk.raw += buf.data();
    }
    hunks.push_back(hunk);
    pclose(pipe);
  }
}

GitView::GitView(const std::string& path)
  : m_path(path) {
  git_status();
}

std::string GitView::get_status_text() const {
  return fmt::format("-- {} --", m_path);
}

void GitView::show(Ctx* ctx) {
  PanelPtr panel = this->panel();
  panel->clear();
  panel->move(0, 0);

  panel->print("-- git status --\n");

  int i = 0;
  for (const auto& item : m_items) {
    if (m_selected == i)
      panel->attr_reverse();
    else
      panel->attr_normal();

    panel->print(item.file.c_str());
    panel->print("\n");

    panel->attr_normal();

    if (item.open) {
      for (const auto& hunk : item.hunks) {
        panel->print(hunk.raw.c_str());
      }
    }
    ++i;
  }

  panel->update();
  View::show(ctx);
}

bool GitView::on_key(Ctx* ctx, const Key& key) {
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
      if (m_selected < int(m_items.size()-1))
        ++m_selected;
      break;
    case Key::Scancode::Tab:
      if (m_selected >= 0 && m_selected < int(m_items.size())) {
        Item& item = m_items[m_selected];
        item.open = !item.open;
        if (item.hunks.empty()) {
          item.fill_hunks();
        }
      }
      break;
  }
  return View::on_key(ctx, key);
}

void GitView::on_search_text(const std::string& text, int skip)
{
  // TODO
}

void GitView::git_status()
{
  m_items.clear();

  auto pipe = popen("git status --porcelain", "r");
  if (pipe) {
    std::vector<char> buf(1024);
    while (fgets(buf.data(), buf.size(), pipe)) {
      if (buf.size() < 4)
        continue;

      // Ignore untracked?
      if (buf[0] == '?')
        continue;

      Item item;
      item.x = buf[0];
      item.y = buf[1];
      base::trim_string(buf.data()+3,
                        item.file);

      m_items.push_back(item);
    }
    pclose(pipe);
  }
}
