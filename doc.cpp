// handy text editor
// Copyright (c) 2016-2017 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "doc.h"

#include "undo/undo_command.h"

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
  : m_filename("untitled")
  , m_saved_state(nullptr) {
}

bool Doc::modified() const {
  return (m_undo.currentState() != m_saved_state);
}

bool Doc::load(const char* fn) {
  m_buf.clear();
  m_filename = fn;

  m_undo = undo::UndoHistory();
  m_saved_state = nullptr;

  std::ifstream f(fn, std::ios::in | std::ios::binary);
  if (!f)
    return false;

  std::vector<char> tmp(4096);
  while (!f.eof()) {
    f.read(&tmp[0], tmp.size());
    if (f.gcount())
      m_buf.append(&tmp[0], f.gcount());
  }

  m_saved_state = m_undo.currentState();
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
    m_saved_state = m_undo.currentState();
  return true;
}

int Doc::get_char(cursor_t i) const {
  return (i >= 0 && i < m_buf.size() ? m_buf[i]: 0);
}

void Doc::insert(const cursor_t pos, const char* buf, const cursor_t n) {
  const std::string tmp(buf, buf+n);
  m_undo.add(
    new Undoable(
      [this, pos, tmp, n]{
        insert_without_undo(pos, tmp.c_str(), tmp.size());
      },
      [this, pos, n]{
        erase_without_undo(pos, n);
      }));
}

void Doc::erase(const cursor_t pos, const cursor_t n) {
  const std::string tmp = m_buf.substr(pos, n);
  m_undo.add(
    new Undoable(
      [this, pos, n]{
        erase_without_undo(pos, n);
      },
      [this, pos, tmp]{
        insert_without_undo(pos, tmp.c_str(), tmp.size());
      }));
}

void Doc::insert_without_undo(const cursor_t pos, const char* buf, const cursor_t n) {
  m_cursors.update_cursors_from(pos, n);
  m_buf.insert(pos, buf, n);

  m_last_modified_pos = pos+n;
}

void Doc::erase_without_undo(const cursor_t pos, const cursor_t n) {
  m_cursors.update_cursors_from(pos+n, -n);
  m_buf.erase(pos, n);

  m_last_modified_pos = pos;
}

void Doc::undo() {
  if (m_undo.canUndo())
    m_undo.undo();
}

void Doc::redo() {
  if (m_undo.canRedo())
    m_undo.redo();
}
