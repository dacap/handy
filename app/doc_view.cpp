// handy text editor
// Copyright (c) 2016-2024 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "doc_view.h"

#include "clipboard.h"
#include "open_file_view.h"

#include <algorithm>

std::string DocView::clipboard;

DocView::DocView(const DocPtr& doc)
  : m_doc(doc) {
  m_cursor_ref = m_doc->cursors().add(0);
  m_sel_ref = -1;
  set_mode(Mode::Ins);
}

void DocView::set_doc(const DocPtr& doc) {
  m_doc = doc;
}

void DocView::set_cursor(cursor_t i) {
  m_doc->cursors().set(m_cursor_ref, i);
  update_scroll();
}

void DocView::set_scroll(const Point& scroll) {
  m_scroll = scroll;
}

std::string DocView::get_status_text() const {
  char buf[2048];
  if (mode() == Mode::Cmd) {
    sprintf(buf, "Command? [s]ave [m]ore [q]uit");
  }
  else {
    Point p = m_doc->convert_cursor_to_point(cursor());
    sprintf(buf, "-- %d:%d -- %s%s%s --",
            p.y+1, p.x,
            doc()->filename().c_str(),
            doc()->modified() ? " (*)": "",
            mode() == Mode::Ins ? " -- editing":
            mode() == Mode::Sel ? " -- selecting": "");
  }
  return buf;
}

void DocView::show(Ctx* ctx) {
  View::show(ctx);

  PanelPtr panel = this->panel();
  int w = panel->width();
  int h = panel->height();
  panel->clear();

  Point p, cursor_p(-1, -1);
  cursor_t i = 0;
  cursor_t j = cursor();

  panel->attr_normal();
  bool reversed = false;
  for (auto ch : m_doc->str()) {
    // Cursor position
    if (i == j)
      cursor_p = p;

    if (m_sel_ref >= 0) {
      if (i >= std::min(cursor(), sel()) &&
          i < std::max(cursor(), sel())) {
        panel->attr_reverse();
        reversed = true;
      }
      else {
        panel->attr_normal();
        reversed = false;
      }
    }

    if (p.y >= m_scroll.y) {
      if (ch == '\n') {
        // Show a reversed char to show that the \n is selected
        if (reversed)
          panel->print(' ');
      }

      if (p.x >= m_scroll.x && p.x < m_scroll.x+w)
        panel->print(ch);
    }

    if (ch == '\n') {
      if (p.x < m_scroll.x)
        panel->print('\n');

      p.x = 0;
      ++p.y;
      if (p.y >= m_scroll.y+h)
        break;
    }
    else
      ++p.x;

    ++i;
  }
  panel->attr_normal();

  if (cursor_p.x < 0 ||
      cursor_p.y < 0)
    cursor_p = p;
  panel->move(cursor_p.x-m_scroll.x,
              cursor_p.y-m_scroll.y);
  panel->update();
}

bool DocView::on_key(Ctx* ctx, const Key& key) {
  // Globals
  if (key.ctrlKey()) {
    switch (key.scancode) {
      case Key::Scancode::KeyF: // Ctrl+F
        search_text(ctx);
        return true;
      case Key::Scancode::KeyQ: // Ctrl+Q
        quit(ctx);
        return true;
      case Key::Scancode::KeyS: // Ctrl+S
        doc()->save();
        return true;
      case Key::Scancode::KeyW: // Ctrl+W
        close(ctx);
        return true;
    }
  }

  if (mode() == Mode::Nav ||
      mode() == Mode::Sel ||
      // Alt key to escape Ins mode by one key
      (mode() == Mode::Ins && key.altKey())) {
    switch (key.scancode) {

      case Key::Scancode::KeyJ:
        if (key.shiftKey()) {   // Shift+J
          prev_expr();
          return true;
        }
        [[fallthrough]];
      case Key::Scancode::ArrowLeft:
        prev_char();
        return true;

      case Key::Scancode::KeyL:
        if (key.shiftKey()) {   // Shift+L
          next_expr();
          return true;
        }
        [[fallthrough]];
      case Key::Scancode::ArrowRight:
        next_char();
        return true;

      case Key::Scancode::KeyI:
        if (key.shiftKey()) {   // Shift+I
          prev_page();
          return true;
        }
        [[fallthrough]];
      case Key::Scancode::ArrowUp:
        prev_line();
        return true;

      case Key::Scancode::KeyK:
        if (key.shiftKey()) {   // Shift+K
          next_page();
          return true;
        }
        [[fallthrough]];
      case Key::Scancode::ArrowDown:
        next_line();
        return true;

      case Key::Scancode::PageUp:
        prev_page();
        return true;

      case Key::Scancode::PageDown:
        next_page();
        return true;

      case Key::Scancode::KeyU:
        if (key.shiftKey())
          prev_block();
        else
          prev_word();
        return true;

      case Key::Scancode::KeyO:
        if (key.shiftKey())
          next_block();
        else
          next_word();
        return true;

      case Key::Scancode::KeyM:
        set_mode(Mode::Cmd);
        return true;

      case Key::Scancode::KeyW:
        if (key.shiftKey())     // Alt+Shift+W = Close file
          close(ctx);
        else
          clean_whitespace();
        return true;
    }
  }

  if (mode() == Mode::Nav) {
    switch (key.scancode) {
      case Key::Scancode::KeyQ:
        quit(ctx);
        return true;
      case Key::Scancode::KeyS:
        set_mode(Mode::Sel);
        m_sel_ref = m_doc->cursors().add(cursor());
        return true;
      case Key::Scancode::Enter: // Enter starts ins mode
        set_mode(Mode::Ins);
        return true;
      case Key::Scancode::KeyO:
        if (key.ctrlKey()) {
          ctx->set_view(std::make_shared<OpenFileView>());
          return true;
        }
        break;
      // Space and backspace start ins mode modifying the doc
      case Key::Scancode::Space:
      case Key::Scancode::Backspace:
        set_mode(Mode::Ins);
        break;
      case Key::Scancode::KeyD:
        delete_prev_char();
        return true;
      case Key::Scancode::KeyF:
        delete_next_char();
        return true;
      case Key::Scancode::KeyY:
        search_text(ctx);
        return true;
      case Key::Scancode::KeyH:
        if (key.shiftKey()) {
          if (!end_of_line())
            end_of_file();
        }
        else {
          if (!beg_of_line())
            beg_of_file();
        }
        break;
      case Key::Scancode::KeyV: // V or Ctrl+V = Paste
        m_doc->insert(cursor(), Clipboard::get_content());
        break;
      case Key::Scancode::KeyZ:
        if (key.shiftKey()) {
          // Redo
          m_doc->redo();
          set_cursor(m_doc->last_modified_index());
        }
        else {
          // Undo
          m_doc->undo();
          set_cursor(m_doc->last_modified_index());
        }
        break;
    }
  }

  if (mode() == Mode::Ins) {
    switch (key.scancode) {
      case Key::Scancode::ArrowLeft:  prev_char(); return true;
      case Key::Scancode::ArrowRight: next_char(); return true;
      case Key::Scancode::ArrowUp:    prev_line(); return true;
      case Key::Scancode::ArrowDown:  next_line(); return true;
      case Key::Scancode::PageUp:     prev_page(); return true;
      case Key::Scancode::PageDown:   next_page(); return true;
      case Key::Scancode::Escape:
        set_mode(Mode::Nav);
        break;
      case Key::Scancode::Backspace:
        delete_prev_char();
        break;
      case Key::Scancode::Enter:
        m_doc->insert(cursor(), 10);
        update_scroll(); // TODO call this automatically when m_doc.cursors.update_cursors_from() is signaled
        break;
      default:
        if (key.codepoint && !key.ctrlKey()) {
          m_doc->insert(cursor(), key.codepoint);
          update_scroll(); // TODO call this automatically when m_doc.cursors.update_cursors_from() is signaled
        }
        break;
    }
    return true;
  }

  if (mode() == Mode::Sel) {
    switch (key.scancode) {
      case Key::Scancode::Backspace:
        delete_sel();
        break;
        // continue in 'x' case
      case Key::Scancode::KeyX: // X or Ctrl+X = Cut
        Clipboard::set_content(sel_content());
        delete_sel();
        break;
      case Key::Scancode::KeyC: // C or Ctrl+C = Copy
        Clipboard::set_content(sel_content());
        break;
      case Key::Scancode::KeyQ: // TODO quit
        break;
    }
    m_doc->cursors().del(m_sel_ref);
    m_sel_ref = -1;
    set_mode(Mode::Nav);
  }

  if (mode() == Mode::Cmd) {
    switch (key.scancode) {
      case Key::Scancode::KeyQ:
        quit(ctx);
        return true;
      case Key::Scancode::KeyF:
        ctx->set_view(std::make_shared<OpenFileView>());
        break;
      case Key::Scancode::KeyS:
        doc()->save();
        break;
    }
    set_mode(Mode::Nav);
    return true;
  }

  return View::on_key(ctx, key);
}

void DocView::on_search_text(const std::string& text, int skip)
{
  for (cursor_t i=cursor(); i < m_doc->size(); ++i) {
    cursor_t j = 0;
    for (; j < text.size() &&
           i+j < m_doc->size() &&
           m_doc->get_char(i+j) == text[j]; ++j)
      ;
    if (j == text.size()) {
      if (skip-- == 0) {
        set_cursor(i);
        break;
      }
    }
  }
}

void DocView::close(Ctx* ctx) {
  if (!ask_save_changes(ctx))
    return;

  ctx->close_file(m_doc);
}

void DocView::quit(Ctx* ctx) {
  if (!ask_save_changes(ctx))
    return;

  ctx->close();
}

bool DocView::prev_char() {
  cursor_t i = cursor();
  if (i > 0) {
    --i;
    set_cursor(i);
    return true;
  }
  else
    return false;
}

void DocView::next_char() {
  cursor_t i = cursor();
  if (i < m_doc->size()) {
    ++i;
    set_cursor(i);
  }
}

void DocView::prev_line() {
  cursor_t i = cursor();
  int x = m_doc->convert_cursor_to_point(i).x;
  if (i > 0 && m_doc->get_char(i) == '\n')
    --i;
  while (i > 0 && m_doc->get_char(i) != '\n')
    --i;

  if (i == 0)
    return;

  int u = m_doc->convert_cursor_to_point(i).x;
  if (u > x) {
    while (u > x) {
      --i;
      --u;
    }
  }
  set_cursor(i);
}

void DocView::next_line() {
  cursor_t i = cursor();
  int x = m_doc->convert_cursor_to_point(i).x;
  while (i < m_doc->size() && m_doc->get_char(i) != '\n')
    ++i;

  if (i == m_doc->size())
    return;

  ++i;
  while (x > 0 && i < m_doc->size() && m_doc->get_char(i) != '\n') {
    ++i;
    --x;
  }
  set_cursor(i);
}

void DocView::prev_page() {
  int h = panel()->height()-1;
  while (h-- > 0) {
    prev_line();
  }
}

void DocView::next_page() {
  int h = panel()->height()-1;
  while (h-- > 0) {
    next_line();
  }
}

void DocView::prev_word(int extra_ch) {
  cursor_t i = cursor();
  if (i > 0)
    --i;

  const char* text = m_doc->c_str();
  const char* p = text+i;

  for (; p!=text-1; --p) {
    if ((*p >= 'a' && *p <= 'z') ||
        (*p >= 'A' && *p <= 'Z') ||
        (*p >= '0' && *p <= '9') ||
        (*p == extra_ch)) {
      break;
    }
    else
      continue;
  }

  for (; p!=text-1; --p) {
    if ((*p >= 'a' && *p <= 'z') ||
        (*p >= 'A' && *p <= 'Z') ||
        (*p >= '0' && *p <= '9') ||
        (*p == extra_ch))
      continue;
    else
      break;
  }

  set_cursor(p-text+1);
}

void DocView::next_word(int extra_ch) {
  const char* text = m_doc->c_str();
  const char* p = text+cursor();
  const char* end = text+m_doc->size();

  for (; p!=end; ++p) {
    if ((*p >= 'a' && *p <= 'z') ||
        (*p >= 'A' && *p <= 'Z') ||
        (*p >= '0' && *p <= '9') ||
        (*p == extra_ch))
      break;
    else
      continue;
  }

  for (; p!=end; ++p) {
    if ((*p >= 'a' && *p <= 'z') ||
        (*p >= 'A' && *p <= 'Z') ||
        (*p >= '0' && *p <= '9') ||
        (*p == extra_ch))
      continue;
    else
      break;
  }

  set_cursor(p-text);
}

void DocView::prev_expr() {
  prev_word('_');
}

void DocView::next_expr() {
  next_word('_');
}

void DocView::prev_block() {
}

void DocView::next_block() {
}

void DocView::delete_prev_char() {
  cursor_t i = cursor();
  if (i > 0) {
    m_doc->erase(i-1);
    update_scroll(); // TODO call this automatically when m_doc.cursors.update_cursors_from() is signaled
  }
}

void DocView::delete_next_char() {
  cursor_t i = cursor();
  if (i < m_doc->size()) {
    m_doc->erase(i);
    update_scroll(); // TODO call this automatically when m_doc.cursors.update_cursors_from() is signaled
  }
}

bool DocView::delete_sel() {
  if (m_sel_ref < 0)
    return false; // No selection

  cursor_t i = std::min(cursor(), sel());
  cursor_t j = std::max(cursor(), sel());
  return delete_range(i, j);
}

bool DocView::delete_range(cursor_t i, cursor_t j) {
  if (i < j) {
    m_doc->erase(i, j-i);
    set_cursor(i);
    update_scroll(); // TODO call this automatically when m_doc.cursors.update_cursors_from() is signaled
    return true;
  }
  else
    return false;
}

bool DocView::beg_of_line() {
  cursor_t i = cursor();
  if (i > 0)
    --i;
  while (i > 0) {
    if (m_doc->get_char(i) != '\n')
      --i;
    else {
      ++i;
      break;
    }
  }
  if (i != cursor()) {
    set_cursor(i);
    return true;
  }
  else
    return false;
}

bool DocView::end_of_line() {
  cursor_t i = cursor();
  while (i < m_doc->size()) {
    if (m_doc->get_char(i) != '\n')
      ++i;
    else
      break;
  }
  if (i != cursor()) {
    set_cursor(i);
    return true;
  }
  else
    return false;
}

void DocView::beg_of_file() {
  set_cursor(0);
}

void DocView::end_of_file() {
  set_cursor(m_doc->size());
}

void DocView::clean_whitespace() {
  cursor_t i, j, ni, nj;
  i = j = cursor();
  ni = nj = -1;

  // Find bol
  if (i > 0)
    --i;
  while (i > 0) {
    int ch = m_doc->get_char(i);
    if (ch == '\n') {
      ni = ++i;
      break;
    }
    else if (ch != ' ' && ch != '\t') {
      ++i;
      break;
    }
    --i;
  }

  // Find eol
  while (j < m_doc->size()) {
    int ch = m_doc->get_char(j);
    if (ch == '\n') {
      nj = j;
      break;
    }
    else if (ch != ' ' && ch != '\t')
      break;
    ++j;
  }

  // Delete several lines
  const bool multi_lines = (ni >= 0 && nj >= 0);
  if (multi_lines) {
    int newline_chars = 0;

    // Delete one line
    while (i > 0) {
      int ch = m_doc->get_char(i);
      if (ch == '\n') {
        ni = i+1;
        ++newline_chars;
      }
      else if (ch != ' ' && ch != '\t') {
        i = ni;
        break;
      }
      --i;
    }
    while (j < m_doc->size()) {
      int ch = m_doc->get_char(j);
      if (ch == '\n') {
        nj = j;
        ++newline_chars;
      }
      else if (ch != ' ' && ch != '\t') {
        j = nj + (newline_chars > 3 ? 0: 1);
        break;
      }
      ++j;
    }
  }

  if (i != j) {
    if (multi_lines)
      delete_range(i, j);
    else {
      if (j-i > 1)
        delete_range(i, j-1);
      else
        delete_range(i, j);
    }
  }
}

bool DocView::ask_save_changes(Ctx* ctx) {
  if (m_doc->modified()) {
    const Key key = ctx->alert("Quit => Save changes? [y]es [n]o [c]ancel");
    if (key.scancode == Key::Scancode::KeyY)
      m_doc->save();
    else if (key.scancode != Key::Scancode::KeyN)
      return false;             // Cancel
  }
  return true;
}

std::string DocView::sel_content()
{
  cursor_t i = std::min(cursor(), sel());
  cursor_t j = std::max(cursor(), sel());
  return m_doc->str().substr(i, j-i);
}

void DocView::update_scroll() {
  // Update scroll
  PanelPtr panel = this->panel();
  Point p = m_doc->convert_cursor_to_point(cursor());
  Point scroll = this->scroll();
  if (p.x < scroll.x) scroll.x = p.x;
  if (p.y < scroll.y) scroll.y = p.y;
  if (p.x > scroll.x+panel->width()-1) scroll.x = p.x-panel->width()+1;
  if (p.y > scroll.y+panel->height()-1) scroll.y = p.y-panel->height()+1;
  set_scroll(scroll);
}
