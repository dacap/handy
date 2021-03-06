// handy text editor
// Copyright (c) 2016-2021 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include <memory>

class Doc;
class Panel;
class Term;
class View;

using DocPtr = std::shared_ptr<Doc>;
using PanelPtr = std::shared_ptr<Panel>;
using TermPtr = std::shared_ptr<Term>;
using ViewPtr = std::shared_ptr<View>;

using cursor_t = int;
using cursor_ref = int;
