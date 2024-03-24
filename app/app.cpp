// handy text editor
// Copyright (c) 2016-2024 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "app.h"

#include "alert_view.h"
#include "app.h"
#include "base.h"
#include "base/fs.h"
#include "ctx.h"
#include "doc_view.h"
#include "event.h"
#include "lua.h"
#include "view.h"

#include <cstring>

App::App(int argc, char* argv[])
  : m_lua(new Lua)
  , m_term(nullptr) {
  m_running = true;

  for (int i=0; i<argc; ++i) {
    if (std::strcmp(argv[i], "-tui") == 0) {
      m_term = Term::MakeTUI();
    }
    else if (std::strcmp(argv[i], "-gui") == 0) {
#if LAF_SKIA
      m_term = Term::MakeGUI();
#else
      std::printf("There isn't GUI support");
      exit(1);
#endif
    }
  }

  if (!m_term) {
#if LAF_SKIA
    m_term = Term::MakeGUI();
#else
    m_term = Term::MakeTUI();
#endif
  }

  m_main = m_term->makePanel(0, 0, m_term->width(), m_term->height()-1);
  m_status = m_term->makePanel(0, m_term->height()-1, m_term->width(), 1);
  m_panels.push_back(m_main);
  m_panels.push_back(m_status);

  auto main_view = std::make_shared<DocView>(std::make_shared<Doc>());
  m_views.push(main_view);
  main_view->set_panel(m_main);

  m_lua->run_script(base::get_file_title_with_path(argv[0]) + ".lua");
  m_lua->run_code("if init then init() end");
}

App::~App()
{
  m_lua->run_code("if exit then exit() end");
}

// Ctx impl
void App::close() {
  m_running = false;
}

PanelPtr App::status() {
  return m_status;
}

ViewPtr App::view() {
  return m_views.top();
}

void App::set_view(const ViewPtr& v) {
  v->set_panel(view()->panel());
  view()->set_panel(nullptr);
  m_views.push(v);
}

void App::back_view() {
  PanelPtr panel = view()->panel();
  m_views.pop();
  view()->set_panel(panel);
}

Key App::alert(const char* msg) {
  auto alert_view = std::make_shared<AlertView>(msg);
  set_view(alert_view);
  while (is_running() && view() == alert_view)
    loop();
  return alert_view->result();
}

bool App::is_running() const {
  return m_running;
}

void App::loop() {
  ViewPtr view = this->view();
  view->show(this);

  Event ev = view->panel()->get_event();

  if (ev.type() == Event::Type::Key)
    view->on_key(this, ev.key());
}

void App::open_file(const char* fn) {
  auto doc = std::make_shared<Doc>();
  if (doc->load(fn))
    set_view(std::make_shared<DocView>(doc));
}
