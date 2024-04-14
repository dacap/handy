// handy text editor
// Copyright (c) 2024 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "keys.h"

class Event {
public:
  enum class Type {
    Unknown,
    Resize,
    Key,
  };

  Event(Type type = Type::Unknown)
    : m_type(type) { }

  Type type() const { return m_type; }
  Key key() const { return m_key; }

  void key(const Key& key) { m_key = key; }

private:
  Type m_type;
  Key m_key;
};
