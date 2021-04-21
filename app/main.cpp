// handy text editor
// Copyright (c) 2016-2021 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "app.h"
#include "base/fs.h"

int main(int argc, char* argv[]) {
  App app(argc, argv);

  for (int i=1; i<argc; ++i)
    if (base::is_file(argv[i]))
      app.open_file(argv[i]);

  while (app.is_running())
    app.loop();
}
