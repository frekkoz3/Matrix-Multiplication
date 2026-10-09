#pragma once

#include <cstddef>

struct Arguments
{
    std::size_t size = 1024;
    int iterations = 1;
    bool verify = true;
};

Arguments parseArguments(int argc, char* argv[]);

void printHelp();