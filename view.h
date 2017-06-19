// handy text editor
// Copyright (c) 2016-2017 David Capello
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
  enum class Mode {
    Nav,                        // Navigation
    Ins,                        // Inserting/editing text
    Cmd,                        // Calling a command by hand
    Sel,                        // Selecting text
    Sea                         // Searching text
  };

  View() : m_mode(Mode::Nav) { }
  virtual ~View() { }

  PanelPtr panel() const { return m_panel; }
  void set_panel(const PanelPtr& panel) {
    m_panel = panel;
  }

  Mode mode() const { return m_mode; }
  void set_mode(const Mode& mode) {
    m_mode = mode;
  }

  virtual std::string get_status_text() const {
    return "";
  }

  virtual void show(Ctx* ctx) {
    ctx->status()->clear();

    if (mode() == Mode::Sea) {
      ctx->status()->print(("Searching: " + m_searchingText).c_str());
    }
    else {
      ctx->status()->print(get_status_text().c_str());
    }

    ctx->status()->update();
  }

  virtual bool on_key(Ctx* ctx, int ch) {
    // Searching text
    if (mode() == Mode::Sea) {
      switch (ch) {
        case 27:
          set_mode(Mode::Nav);  // Back to navigation
          return true;
        default:
          m_searchingText.push_back(ch);
          break;
      }
    }

    return false;
  }

  void search_text(Ctx* ctx) {
    set_mode(Mode::Sea);
    m_searchingText = "";
  }

private:
  PanelPtr m_panel;
  Mode m_mode;
  std::string m_searchingText;
};
