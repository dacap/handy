// handy text editor
// Copyright (c) 2016-2017 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "doc.h"

Doc::Doc() {
  m_modified = false;
  m_filename = "untitled";
}

bool Doc::load(const char* fn) {
  m_buf.clear();
  m_filename = fn;
  m_modified = false;

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
  m_modified = false;
  return true;
}

int Doc::get_char(cursor_t i) const {
  return (i >= 0 && i < m_buf.size() ? m_buf[i]: 0);
}

void Doc::insert(cursor_t pos, char ch) {
  m_modified = true;
  m_cursors.update_cursors_from(pos, 1);
  m_buf.insert(pos, 1, ch);
}

void Doc::erase(cursor_t pos, cursor_t count) {
  m_modified = true;
  m_cursors.update_cursors_from(pos+count, -count);
  m_buf.erase(pos, count);
}
