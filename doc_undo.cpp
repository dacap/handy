// handy text editor
// Copyright (c) 2017 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "doc_undo.h"

DocUndo::DocUndo()
  : m_saved_state(nullptr) {
}

bool DocUndo::in_saved_state() const {
  return (m_undo.currentState() != m_saved_state);
}

void DocUndo::clear() {
  m_undo = undo::UndoHistory();
  m_saved_state = nullptr;
}

void DocUndo::mark_as_saved_state() {
  m_saved_state = m_undo.currentState();
}

void DocUndo::add(undo::UndoCommand* cmd) {
  m_undo.add(cmd);
}
