// handy text editor
// Copyright (c) 2016-2024 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "alert_view.h"
#include "app.h"
#include "base.h"
#include "ctx.h"
#include "doc_view.h"
#include "view.h"

#include <memory>
#include <vector>

class Lua;

class App : public Ctx {
public:
  App(int argc, char* argv[]);
  ~App();

  // Ctx impl
  void close() override;
  PanelPtr status() override;
  ViewPtr view() override;
  void set_view(const ViewPtr& v) override;
  void back_view() override;
  Key alert(std::string_view msg) override;
  bool is_running() const;
  void loop();
  void open_file(const char* fn) override;
  void close_file(const DocPtr& doc) override;

private:
  void recreate_panels();
  void make_new_untitled();

  bool m_running;
  TermPtr m_term;
  std::vector<PanelPtr> m_panels;
  PanelPtr m_main, m_status;
  std::vector<ViewPtr> m_views;
  std::unique_ptr<Lua> m_lua;
};
