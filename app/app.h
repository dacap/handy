// handy text editor
// Copyright (c) 2016-2017 David Capello
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

#include <stack>
#include <vector>

class App : public Ctx {
public:

  App();

  // Ctx impl
  void close() override;
  PanelPtr status() override;
  ViewPtr view() override;
  void set_view(const ViewPtr& v) override;
  void back_view() override;
  int alert(const char* msg) override;
  bool is_running() const;
  void loop();
  void open_file(const char* fn) override;

private:
  bool m_running;
  Term m_term;
  std::vector<PanelPtr> m_panels;
  PanelPtr m_main, m_status;
  std::stack<ViewPtr> m_views;
};
