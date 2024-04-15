// handy text editor
// Copyright (c) 2024 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "view.h"

#include <vector>

class GitView : public View {
public:
  GitView(const std::string& path);

  std::string get_status_text() const override;

  void show(Ctx* ctx) override;
  bool on_key(Ctx* ctx, const Key& key) override;
  void on_search_text(const std::string& text, int skip) override;

private:
  struct HunkRange {
    int line;
    int nlines;
  };

  struct Hunk {
    HunkRange from, to;
    std::string raw;
  };
  using Hunks = std::vector<Hunk>;

  struct Item {
    char x, y;
    std::string file;
    bool open = false;
    Hunks hunks;

    void fill_hunks();
  };
  using Items = std::vector<Item>;

  void git_status();

  std::string m_path;
  Items m_items;
  int m_selected = 0;
};
