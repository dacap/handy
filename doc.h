// handy text editor
// Copyright (c) 2016-2017 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "cursors.h"

#include <fstream>
#include <string>

class Doc {
public:
  Doc();

  const std::string& filename() const { return m_filename; }
  bool modified() const { return m_modified; }

  bool load(const char* fn);
  bool save();
  bool save(const char* fn);

  const std::string& str() const { return m_buf; }
  std::size_t size() const { return m_buf.size(); }
  const char* c_str() const { return m_buf.c_str(); }

  int get_char(cursor_t i) const;
  void insert(cursor_t pos, char ch);
  void erase(cursor_t pos, cursor_t count = 1);

  Cursors& cursors() { return m_cursors; }

private:
  std::string m_filename;
  std::string m_buf;
  bool m_modified;
  Cursors m_cursors;
};
