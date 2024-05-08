// handy text editor
// Copyright (c) 2016-2024 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "base/fs.h"
#include "point.h"
#include "view.h"

class DirView : public View {
public:
  DirView();

  std::string get_status_text() const override;

  void show(Ctx* ctx) override;
  bool on_key(Ctx* ctx, const Key& key) override;
  void on_search_text(const std::string& text, int skip) override;

private:
  void open_path(const std::string& path);
  void update_scroll();

  std::string m_path;
  std::vector<std::string> m_files;
  int m_selected;
  Point m_scroll;
};
