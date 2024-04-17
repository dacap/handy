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

#include <cassert>
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

  recreate_panels();
  make_new_untitled();

  m_lua->run_script(this, base::get_file_title_with_path(argv[0]) + ".lua");
  m_lua->run_code(this, "if init then init() end");
}

App::~App()
{
  m_lua->run_code(this, "if exit then exit() end");
}

// Ctx impl
void App::close() {
  while (!m_views.empty()) {
    ViewPtr view = this->view();
    if (view->on_close(this))
      m_views.pop_back();
    else
      return;                   // Operation canceled
  }
  m_running = false;
}

PanelPtr App::status() {
  return m_status;
}

ViewPtr App::view() {
  return (!m_views.empty() ? m_views.back(): ViewPtr());
}

void App::set_view(const ViewPtr& v) {
  v->set_panel(m_main);

  if (auto old_view = view())
    old_view->set_panel(nullptr);

  m_views.push_back(v);
}

void App::back_view() {
  assert(view());
  if (!view())
    return;

  m_views.pop_back();
  if (auto new_view = view())
    new_view->set_panel(m_main);
}

Key App::alert(std::string_view msg) {
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
  if (!view->panel()) // TODO add assert, this shouldn't happen
    view->set_panel(m_main);

  view->show(this);

  Event ev = view->panel()->get_event();

  switch (ev.type()) {

    case Event::Type::Close:
      close();
      break;

    case Event::Type::Resize:
      recreate_panels();
      break;

    case Event::Type::Key:
      view->on_key(this, ev.key());
      break;
  }
}

void App::open_file(const char* fn) {
  auto doc = std::make_shared<Doc>();
  if (doc->load(fn))
    set_view(std::make_shared<DocView>(doc));
}

void App::close_file(const DocPtr& doc) {
  for (auto it=m_views.begin(), end=m_views.end();
       it != end; ) {
    if (auto* docView = dynamic_cast<DocView*>(it->get())) {
      if (docView->doc() == doc) {
        (*it)->set_panel(nullptr);

        it = m_views.erase(it);
        end = m_views.end();
        continue;
      }
    }
    ++it;
  }

  if (m_views.empty()) {
    make_new_untitled();
  }
  else {
    set_view(m_views.back());
  }
}

void App::recreate_panels() {
  PanelPtr old_main = m_main;

  m_main = m_term->makePanel(0, 0, m_term->width(), m_term->height()-1);
  m_status = m_term->makePanel(0, m_term->height()-1, m_term->width(), 1);
  m_panels.clear();
  m_panels.push_back(m_main);
  m_panels.push_back(m_status);

  for (auto& view : m_views) {
    if (view->panel() == old_main)
      view->set_panel(m_main);
  }
}

void App::make_new_untitled() {
  set_view(std::make_shared<DocView>(std::make_shared<Doc>()));
}
