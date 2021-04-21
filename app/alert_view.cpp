// handy text editor
// Copyright (c) 2016-2018 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "alert_view.h"

AlertView::AlertView(const char* msg) {
  m_msg = msg;
  m_result = 0;
}

int AlertView::result() const {
  return m_result;
}

std::string AlertView::get_status_text() const {
  return m_msg;
}

void AlertView::show(Ctx* ctx) {
  View::show(ctx);
  ctx->status()->move(m_msg.size()+1, 0);
  ctx->status()->update();
}

bool AlertView::on_key(Ctx* ctx, int ch) {
  View::on_key(ctx, ch);
  m_result = ch;
  ctx->back_view();
  return true;
}
