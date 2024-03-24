// handy text editor
// Copyright (c) 2016-2024 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "alert_view.h"

AlertView::AlertView(const char* msg) {
  m_msg = msg;
}

std::string AlertView::get_status_text() const {
  return m_msg;
}

void AlertView::show(Ctx* ctx) {
  View::show(ctx);
  ctx->status()->move(m_msg.size()+1, 0);
  ctx->status()->update();
}

bool AlertView::on_key(Ctx* ctx, const Key& key) {
  View::on_key(ctx, key);
  m_result = key;
  ctx->back_view();
  return true;
}
