// handy text editor
// Copyright (c) 2016-2018 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "base/fs.h"
#include "view.h"

class OpenFileView : public View {
public:
  OpenFileView();

  std::string get_status_text() const override;

  void show(Ctx* ctx) override;
  bool on_key(Ctx* ctx, int ch) override;
  void on_search_text(const std::string& text, int skip) override;

private:
  std::string m_path;
  std::vector<std::string> m_files;
  int m_selected;
};
