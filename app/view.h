// handy text editor
// Copyright (c) 2016-2024 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "base.h"
#include "ctx.h"
#include "keys.h"
#include "term.h"

#include <stack>
#include <string>

class View {
public:
  enum class Mode {
    Nav,                        // Navigation
    Ins,                        // Inserting/editing text
    Cmd,                        // Calling a command by hand
    Sel,                        // Selecting text
    Sea                         // Searching text
  };

  View();
  virtual ~View();

  PanelPtr panel() const { return m_panel; }
  void set_panel(const PanelPtr& panel) {
    m_panel = panel;
  }

  Mode mode() const { return m_mode; }
  void set_mode(Mode mode) {
    m_mode = mode;
  }
  void enter_mode(Mode mode);
  void back_mode(Mode last_mode);

  virtual std::string get_status_text() const;
  virtual void show(Ctx* ctx);
  virtual bool on_key(Ctx* ctx, const Key& key);
  virtual void on_search_text(const std::string& text, int skip = 0) { }
  virtual bool on_close(Ctx* ctx) { return true; }

  void search_text(Ctx* ctx);

private:
  PanelPtr m_panel;
  Mode m_mode;
  std::stack<Mode> m_old_modes;
  std::string m_searching_text;
};
