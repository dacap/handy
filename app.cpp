// handy text editor
// Copyright (c) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "app.h"

#include "alert_view.h"
#include "app.h"
#include "base.h"
#include "ctx.h"
#include "doc_view.h"
#include "view.h"

App::App() {
  m_running = true;

  m_main.reset(new Panel(0, 0, m_term.width(), m_term.height()-1));
  m_status.reset(new Panel(0, m_term.height()-1, m_term.width(), 1));
  m_panels.push_back(m_main);
  m_panels.push_back(m_status);

  ViewPtr main_view(new DocView(DocPtr(new Doc)));
  m_views.push(main_view);
  main_view->set_panel(m_main);
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

int App::alert(const char* msg) {
  ViewPtr alert_view(new AlertView(msg));
  set_view(alert_view);
  while (is_running() && view() == alert_view)
    loop();
  return ((AlertView*)alert_view.get())->result();
}

bool App::is_running() const {
  return m_running;
}

void App::loop() {
  ViewPtr view = this->view();
  view->show(this);

  int ch = view->panel()->get_char();

  view->on_key(this, ch);
}

void App::load_file(const char* fn) {
  DocPtr doc(new Doc);
  if (doc->load(fn))
    set_view(ViewPtr(new DocView(doc)));
}
