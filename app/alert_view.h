// handy text editor
// Copyright (c) 2016-2018 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "view.h"

#include <string>

class AlertView : public View {
public:
  AlertView(const char* msg);
  int result() const;

  std::string get_status_text() const override;
  void show(Ctx* ctx) override;
  bool on_key(Ctx* ctx, int ch) override;

private:
  std::string m_msg;
  int m_result;
};
