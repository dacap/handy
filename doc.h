// handy text editor
// Copyright (c) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "cursors.h"

#include <fstream>
#include <string>

class Doc {
public:

  Doc() {
    m_modified = false;
    m_filename = "untitled";
  }

  const std::string& filename() const { return m_filename; }
  bool modified() const { return m_modified; }

  bool load(const char* fn) {
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

  bool save() {
    return save(m_filename.c_str());
  }

  bool save(const char* fn) {
    std::ofstream f(fn);
    if (!f)
      return false;
    f.write(c_str(), m_buf.size());
    m_modified = false;
    return true;
  }

  const std::string& str() const {
    return m_buf;
  }

  std::size_t size() const {
    return m_buf.size();
  }

  const char* c_str() const {
    return m_buf.c_str();
  }

  int get_char(cursor_t i) const {
    return (i >= 0 && i < m_buf.size() ? m_buf[i]: 0);
  }

  void insert(cursor_t pos, char ch) {
    m_modified = true;
    m_cursors.update_cursors_from(pos, 1);
    m_buf.insert(pos, 1, ch);
  }

  void erase(cursor_t pos, cursor_t count = 1) {
    m_modified = true;
    m_cursors.update_cursors_from(pos+count, -count);
    m_buf.erase(pos, count);
  }

  Cursors& cursors() {
    return m_cursors;
  }

private:
  std::string m_filename;
  std::string m_buf;
  bool m_modified;
  Cursors m_cursors;
};
