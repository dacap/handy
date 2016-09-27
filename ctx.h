// handy text editor
// Copyright (c) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "base.h"

class Ctx {
public:
  virtual ~Ctx() { }
  virtual void close() = 0;
  virtual PanelPtr status() = 0;
  virtual ViewPtr view() = 0;
  virtual void set_view(const ViewPtr& view) = 0;
  virtual void back_view() = 0;
  virtual int alert(const char* msg) = 0;
};
