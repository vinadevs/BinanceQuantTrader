#include "pch.h"
#include "FileUtils.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include <boost/algorithm/string.hpp>

std::string FileUtils::ReadFileContent(const std::string& file)
{
    std::ifstream ifs(file);
    if (!ifs.is_open())
	{
        throw std::runtime_error("Could not open file: " + file);
    }
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    return buffer.str();
}

std::vector<std::string> FileUtils::ReadFileContentToLines(
	const std::string& file,
	const bool trimLine)
{
	std::ifstream ifs(file);
	if (!ifs.is_open()) 
	{
		throw std::runtime_error("Could not open file: " + file);
	}
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(ifs, line))
	{
		if (trimLine)
		{
			boost::algorithm::trim(line);
		}
		lines.push_back(line);
	}
	return lines;
}
