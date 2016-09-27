// handy text editor
// Copyright (c) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "fs.h"
#include "view.h"

class OpenFileView : public View {
public:
  OpenFileView() {
    m_dir = get_current_path();
    m_files = list_files(m_dir);
  }

  std::string get_status_text() const override {
    return "-- " + m_dir + " --";
  }

  void show(Ctx* ctx) override {
    panel()->clear();
    panel()->move(0, 0);
    for (const auto& f : list_files(m_dir)) {
      panel()->print((f+"\n").c_str());
    }
    panel()->update();
    View::show(ctx);
  }

  bool on_key(Ctx* ctx, int ch) override {
    View::on_key(ctx, ch);
    if (ch == 'q' || ch == KEY_ESC) {
      ctx->back_view();
      return true;
    }
    return false;
  }

private:
  std::string m_dir;
  std::vector<std::string> m_files;
};
