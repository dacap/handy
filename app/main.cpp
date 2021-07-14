// handy text editor
// Copyright (c) 2016-2021 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "app.h"
#include "base/fs.h"

#if LAF_SKIA
int app_main(int argc, char* argv[])
#else
int main(int argc, char* argv[])
#endif
{
  App app(argc, argv);

  for (int i=1; i<argc; ++i)
    if (base::is_file(argv[i]))
      app.open_file(argv[i]);

  while (app.is_running())
    app.loop();

  return 0;
}
