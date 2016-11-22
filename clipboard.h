// handy text editor
// Copyright (c) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include <string>

class Clipboard {
public:
  static void set_content(const std::string& text);
  static std::string get_content();

private:
  static std::string m_content;
};
