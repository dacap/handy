// handy text editor
// Copyright (c) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "base.h"
#include "ctx.h"
#include "term.h"

#include <string>

class View {
public:
  virtual ~View() { }

  PanelPtr panel() const { return m_panel; }
  void set_panel(const PanelPtr& panel) {
    m_panel = panel;
  }

  virtual std::string get_status_text() const {
    return "";
  }

  virtual void show(Ctx* ctx) {
    ctx->status()->clear();
    ctx->status()->print(get_status_text().c_str());
    ctx->status()->update();
  }

  virtual bool on_key(Ctx* ctx, int ch) {
    return false;
  }

private:
  PanelPtr m_panel;
};
