# handy text editor
*Copyright (C) 2016-2024 David Capello*

[![build](https://github.com/dacap/handy/actions/workflows/build.yml/badge.svg)](https://github.com/dacap/handy/actions/workflows/build.yml)
[![MIT Licensed](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE.txt)

An experimental text editor. This code is unstable, untested, used
only for experimentation by David Capello.

## Concepts

* [Doc](app/doc.h): a open document, associated to a file in disk when the document is loaded from a file/saved to a file.
* [View](app/view.h): a view to visualize a document ([DocView](app/doc_view.h)), a browsing a file ([DirView](app/dir_view.h)), etc.
* [Mode](app/view.h): a view can be in one of the following modes:
  * `Mode::Nav`: Navigating document/view.
  * `Mode::Ins`: Inserting/editing text in the document.
  * `Mode::Cmd`: Calling a command by hand (entering command name).
  * `Mode::Sel`: Selecting text.
  * `Mode::Sea`: Searching text.

## UX

Some random notes about the "handy" UX:

* Start on editing mode (DocView+Ins mode) when the program is launched with a file name as argument.
* Start on with a file browsing interface (DirView) showing the current directory when the program is started without arguments.
  From this view it should be easy to create new files in the directory or navigate to other directories, open files, etc.
* Editing mode by default with access to special modes with Ctrl+common shortcut (like `Ctrl+F` to find text in the document).
* [Git views](app/git_view.h): status, log, commit, and diff views.
* Zip views: list files inside, view files, extract files (re-compress?).
* General shortcuts guidelines:
  * Similar layout to [handy-mode](https://github.com/superhandy/intro) + modes
  * `Ctrl+Q`: closes the program (ask for saving changes)
  * `Nav`: `J`/`I`/`L`/`K`=previous char/line and next char/line, `Enter`/`Space`/`Backspace`=enter to `Ins` mode
  * `Ins`: `Esc`=go back to `Nav` mode
  * `Sel`: `Enter`=finish selection
  * `Sea`: `Ctrl+F`=go to next result, `Enter`=exit `Sea` mode

## Platforms

This program is being developed on macOS. There are no short-term
plans to port it to other platforms yet.

## Credits

handy depends on the following open source projects:

* [clip](https://github.com/dacap/clip) - Cross-platform C++ library to copy/paste clipboard content
* [fmt](https://github.com/fmtlib/fmt) - A modern formatting library
* [laf](https://github.com/aseprite/laf) - A C++ library to create desktop applications
* [lua](http://www.lua.org) - A Scripting Language
* [undo](https://github.com/dacap/undo) - C++ library to implement non-linear undo/redo functionality
