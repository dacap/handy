// handy text editor
// Copyright (c) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#include "fs.h"

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define MAXPATHLEN 1024

bool is_file(const std::string& path) {
  struct stat sts;
  return (stat(path.c_str(), &sts) == 0 && S_ISREG(sts.st_mode)) ? true: false;
}

std::string get_current_path() {
  std::vector<char> path(MAXPATHLEN);
  getcwd(&path[0], path.size());
  return std::string(&path[0]);
}

std::vector<std::string> list_files(const std::string& path) {
  std::vector<std::string> files;
  DIR* handle = opendir(path.c_str());
  if (handle) {
    dirent* item;
    while ((item = readdir(handle)) != nullptr) {
      std::string filename = item->d_name;
      if (filename != "." && filename != "..")
        files.push_back(filename);
    }

    closedir(handle);
  }
  return files;
}
