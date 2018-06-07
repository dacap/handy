// handy text editor
// Copyright (c) 2016-2017 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "cursors.h"
#include "doc.h"
#include "open_file_view.h"
#include "pos.h"
#include "view.h"

#include <string>

class DocView : public View {
  static std::string clipboard;
public:
  DocView(const DocPtr& doc);

  DocPtr doc() const { return m_doc; }
  cursor_t cursor() const { return m_doc->cursors().get(m_cursor_ref); }
  cursor_t sel() const { return m_doc->cursors().get(m_sel_ref); }
  int cur_char() const { return m_doc->get_char(cursor()); }
  Pos scroll() const { return m_scroll; }

  void set_doc(const DocPtr& doc);
  void set_cursor(cursor_t p);
  void set_scroll(const Pos& scroll);

  // View impl
  std::string get_status_text() const override;
  Pos calc_pos(cursor_t j) const;
  void show(Ctx* ctx) override;
  bool on_key(Ctx* ctx, int ch) override;
  void on_search_text(const std::string& text, int skip) override;

  void quit(Ctx* ctx);
  bool prev_char();
  void next_char();
  void prev_line();
  void next_line();
  void prev_page();
  void next_page();
  void prev_word(int extra_ch = 0);
  void next_word(int extra_ch = 0);
  void prev_expr();
  void next_expr();
  void prev_block();
  void next_block();
  void delete_prev_char();
  void delete_next_char();
  bool delete_sel();
  bool beg_of_line();
  bool end_of_line();
  void beg_of_file();
  void end_of_file();

private:
  std::string sel_content();
  void update_scroll();

  DocPtr m_doc;
  cursor_ref m_cursor_ref;
  cursor_ref m_sel_ref;
  Pos m_scroll;
};
