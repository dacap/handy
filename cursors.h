// handy text editor
// Copyright (c) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "base.h"

#include <vector>

class Cursors {
public:
  Cursors() { }

  cursor_ref add(cursor_t p) {
    for (cursor_ref i=0; i<m_values.size(); ++i) {
      if (m_values[i] == -1) {
        m_values[i] = p;
        return i;
      }
    }
    m_values.push_back(p);
    return m_values.size()-1;
  }

  void del(cursor_ref ref) {
    m_values[ref] = -1;
  }

  cursor_t get(cursor_ref ref) {
    return m_values[ref];
  }

  void set(cursor_ref ref, cursor_t value) {
    m_values[ref] = value;
  }

  void update_cursors_from(cursor_t from, cursor_t delta) {
    for (auto& p : m_values)
      if (p >= from)
        p += delta;
  }

private:
  std::vector<cursor_t> m_values;
};
