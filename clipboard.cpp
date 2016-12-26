// handy text editor
// Copyright (c) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "clipboard.h"
#include "clip/clip.h"

void Clipboard::set_content(const std::string& text) {
  clip::set_text(text);
}

std::string Clipboard::get_content() {
  std::string value;
  clip::get_text(value);
  return value;
}
