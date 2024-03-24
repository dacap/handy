// handy text editor
// Copyright (c) 2016-2024 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "view.h"

#include <string>

class AlertView : public View {
public:
  AlertView(const char* msg);
  const Key& result() const { return m_result; }

  std::string get_status_text() const override;
  void show(Ctx* ctx) override;
  bool on_key(Ctx* ctx, const Key& key) override;

private:
  std::string m_msg;
  Key m_result;
};
