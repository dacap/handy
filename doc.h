// handy text editor
// Copyright (c) 2016-2018 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "cursors.h"
#include "doc_undo.h"
#include "point.h"

#include <fstream>
#include <string>

class Doc {
public:
  Doc();

  const std::string& filename() const { return m_filename; }
  bool modified() const { return m_undo.in_saved_state(); }

  bool load(const char* fn);
  bool save();
  bool save(const char* fn);

  const std::string& str() const { return m_buf; }
  std::size_t size() const { return m_buf.size(); }
  const char* c_str() const { return m_buf.c_str(); }

  int get_char(const cursor_t i) const;
  void insert(const cursor_t pos, const char* buf, const cursor_t n);
  void insert(const cursor_t pos, const std::string& text) {
    if (!text.empty())
      insert(pos, text.c_str(), text.size());
  }
  void insert(const cursor_t pos, const char ch) {
    insert(pos, &ch, 1);
  }
  void erase(const cursor_t pos, const cursor_t n = 1);
  Cursors& cursors() { return m_cursors; }

  void undo();
  void redo();

  cursor_t last_modified_pos() { return m_last_modified_pos; }
  Point convert_cursor_to_point(cursor_t i);

private:
  void insert_without_undo(const cursor_t pos, const char* buf, const cursor_t n);
  void erase_without_undo(const cursor_t pos, const cursor_t n);

  std::string m_filename;
  std::string m_buf;
  Cursors m_cursors;
  DocUndo m_undo;
  cursor_t m_last_modified_pos;
};
