// handy text editor
// Copyright (c) 2017 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include "undo/undo_history.h"

class DocUndo {
public:
  DocUndo();

  bool in_saved_state() const;
  void clear();
  void mark_as_saved_state();

  void add(undo::UndoCommand* cmd);
  bool canUndo() const { return m_undo.canUndo(); }
  bool canRedo() const { return m_undo.canRedo(); }
  void undo() { m_undo.undo(); }
  void redo() { m_undo.redo(); }

private:
  undo::UndoHistory m_undo;
  const undo::UndoState* m_saved_state;
};
