// handy text editor
// Copyright (c) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "clipboard.h"

// static
std::string Clipboard::m_content;

void Clipboard::set_content(const std::string& text) {
  m_content = text;
}

std::string Clipboard::get_content() {
  return m_content;
}
