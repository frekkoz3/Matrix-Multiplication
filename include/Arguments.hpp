#pragma once

#include <cstddef>
#include <string>

struct Arguments
{
    std::size_t size = 1024;
    int iterations = 1;
    std::string method = "cpu";
};

Arguments parseArguments(int argc, char* argv[]);

void printHelp();