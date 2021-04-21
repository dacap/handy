// handy text editor
// Copyright (c) 2016-2018 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "doc.h"

#include "undo/undo_command.h"

#include <functional>

namespace {

class Undoable : public undo::UndoCommand {
public:
  Undoable(std::function<void()> exe,
           std::function<void()> undo)
    : m_exe(exe), m_undo(undo) {
    exe();
  }
  void undo() override { m_undo(); }
  void redo() override { m_exe(); }
  void dispose() override { }
private:
  std::function<void()> m_exe, m_undo;
};

} // anonymous namespace

Doc::Doc()
  : m_filename("untitled") {
}

bool Doc::load(const char* fn) {
  m_buf.clear();
  m_undo.clear();
  m_filename = fn;

  std::ifstream f(fn, std::ios::in | std::ios::binary);
  if (!f)
    return false;

  std::vector<char> tmp(4096);
  while (!f.eof()) {
    f.read(&tmp[0], tmp.size());
    if (f.gcount())
      m_buf.append(&tmp[0], f.gcount());
  }

  return true;
}

bool Doc::save() {
  return save(m_filename.c_str());
}

bool Doc::save(const char* fn) {
  std::ofstream f(fn);
  if (!f)
    return false;
  f.write(c_str(), m_buf.size());
  if (f.good())
    m_undo.mark_as_saved_state();
  return true;
}

int Doc::get_char(cursor_t i) const {
  return (i >= 0 && i < m_buf.size() ? m_buf[i]: 0);
}

void Doc::insert(const cursor_t i, const char* buf, const cursor_t n) {
  const std::string tmp(buf, buf+n);
  m_undo.add(
    new Undoable(
      [this, i, tmp, n]{
        insert_without_undo(i, tmp.c_str(), tmp.size());
      },
      [this, i, n]{
        erase_without_undo(i, n);
      }));
}

void Doc::erase(const cursor_t i, const cursor_t n) {
  const std::string tmp = m_buf.substr(i, n);
  m_undo.add(
    new Undoable(
      [this, i, n]{
        erase_without_undo(i, n);
      },
      [this, i, tmp]{
        insert_without_undo(i, tmp.c_str(), tmp.size());
      }));
}

void Doc::insert_without_undo(const cursor_t i, const char* buf, const cursor_t n) {
  m_cursors.update_cursors_from(i, n);
  m_buf.insert(i, buf, n);

  m_last_modified_index = i+n;
}

void Doc::erase_without_undo(const cursor_t i, const cursor_t n) {
  m_cursors.update_cursors_from(i+n, -n);
  m_buf.erase(i, n);

  m_last_modified_index = i;
}

void Doc::undo() {
  if (m_undo.canUndo())
    m_undo.undo();
}

void Doc::redo() {
  if (m_undo.canRedo())
    m_undo.redo();
}

Point Doc::convert_cursor_to_point(const cursor_t j) {
  cursor_t i = 0;
  Point p;

  for (auto ch : m_buf) {
    if (i == j)
      break;

    if (ch == '\n') {
      p.x = 0;
      ++p.y;
    }
    else
      ++p.x;

    ++i;
  }

  return p;
}
