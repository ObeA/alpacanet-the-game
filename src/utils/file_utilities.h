#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>

class FileUtilities {
public:
    static std::vector<char> readFile(const std::string& filename);
};
