#include "pch.h"
#include "FileUtils.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

std::string FileUtils::ReadFileContent(const std::string& file)
{
    std::ifstream ifs(file);
    if (!ifs.is_open()) {
        throw std::runtime_error("Could not open file: " + file);
    }
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    return buffer.str();
}
