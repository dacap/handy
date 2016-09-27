// handy text editor
// Copyright (c) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "view.h"

#include <string>

class AlertView : public View {
public:
  AlertView(const char* msg) {
    m_msg = msg;
    m_result = 0;
  }
  int result() const { return m_result; }

  std::string get_status_text() const override {
    return m_msg;
  }

  void show(Ctx* ctx) override {
    View::show(ctx);
    ctx->status()->move(m_msg.size()+1, 0);
    ctx->status()->update();
  }

  bool on_key(Ctx* ctx, int ch) override {
    View::on_key(ctx, ch);
    m_result = ch;
    ctx->back_view();
    return true;
  }
private:
  std::string m_msg;
  int m_result;
};
