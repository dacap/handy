// handy text editor
// Copyright (c) 2016-2017 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "base/fs.h"
#include "view.h"

class OpenFileView : public View {
public:
  OpenFileView() {
    m_path = base::get_current_path();
    m_files = base::list_files(m_path);
    m_selected = 0;
  }

  std::string get_status_text() const override {
    return "-- " + m_path + " --";
  }

  void show(Ctx* ctx) override {
    PanelPtr panel = this->panel();
    panel->clear();
    panel->move(0, 0);

    int i = 0;
    for (const auto& f : base::list_files(m_path)) {
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

  bool on_key(Ctx* ctx, int ch) override {
    View::on_key(ctx, ch);
    switch (ch) {
      case 'q':
      case KEY_ESC:
        ctx->back_view();
        return true;
      case 'i':
      case KEY_UP:
        if (m_selected > 0)
          --m_selected;
        break;
      case 'k':
      case KEY_DOWN:
        if (m_selected < int(m_files.size()-1))
          ++m_selected;
        break;
      case 10:                  // Enter open the selected file
        // open file
        if (m_selected >= 0 && m_selected < int(m_files.size()))
          ctx->open_file(
            base::join_path(m_path, m_files[m_selected]).c_str());
        break;
    }
    return false;
  }

private:
  std::string m_path;
  std::vector<std::string> m_files;
  int m_selected;
};
