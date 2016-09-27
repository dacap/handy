// handy text editor
// Copyright (c) 2016 David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#pragma once

#include <string>
#include <vector>

bool is_file(const std::string& path);
std::string get_current_path();
std::vector<std::string> list_files(const std::string& path);
