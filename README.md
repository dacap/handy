# handy text editor
*Copyright (C) 2016-2018 David Capello*

> Distributed under [MIT license](LICENSE.txt)

An experimental text editor. This code is unstable, untested, used
only for experimentation by David Capello.

## Concepts

* [Doc](doc.h): a open document, associated to a file in disk when the document is loaded from a file/saved to a file.
* [View](view.h): a view to visualize a document ([DocView](doc_view.h)), a browsing a file ([OpenFileView](open_file_view.h)), etc.
* [Mode](view.h): a view can be in one of the following modes:
  * `Mode::Nav`: Navigating document/view.
  * `Mode::Ins`: Inserting/editing text in the document.
  * `Mode::Cmd`: Calling a command by hand (entering command name).
  * `Mode::Sel`: Selecting text.
  * `Mode::Sea`: Searching text.

## Platforms

This program is being developed on macOS. There are no short-term
plans to port it to other platforms yet.
