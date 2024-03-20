// handy text editor
// Copyright (c) 2021 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include <memory>

#ifndef KEY_ESC
  #define KEY_ESC   27
#endif // KEY_ESC

#ifndef KEY_DOWN
  #define KEY_DOWN  258
  #define KEY_UP    259
  #define KEY_LEFT  260
  #define KEY_RIGHT 261
#endif // KEY_DOWN

#ifndef KEY_NPAGE
  #define KEY_NPAGE 338
  #define KEY_PPAGE 339
#endif

class Panel;
class Term;

using PanelPtr = std::shared_ptr<Panel>;
using TermPtr = std::shared_ptr<Term>;
