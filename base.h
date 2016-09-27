// handy text editor
// Copyright (c) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include <memory>

class Doc;
class Panel;
class View;

typedef std::shared_ptr<Doc> DocPtr;
typedef std::shared_ptr<Panel> PanelPtr;
typedef std::shared_ptr<View> ViewPtr;

typedef int cursor_t;
typedef int cursor_ref;
