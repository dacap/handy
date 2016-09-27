// handy text editor
// Copyright (c) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "app.h"

int main(int argc, char* argv[]) {
  App app;

  for (int i=1; i<argc; ++i)
    if (is_file(argv[i]))
      app.load_file(argv[i]);

  while (app.is_running())
    app.loop();
}
