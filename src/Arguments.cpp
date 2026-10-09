/*
    Simple Parser for the main entrypoint.
    Right now it include:
    --size (square matrix assumption)
    --iterations (number of times to perform the multiplication)
    --no-verify (if verifying the final value or not)
    --help (to visualize this message)
*/
#include "Arguments.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

namespace
{
    std::size_t parseSize(const std::string& value)
    {
        try
        {
            std::size_t position = 0;
            const auto result = std::stoull(value, &position);

            if (position != value.size() || result == 0)
            {
                throw std::invalid_argument("invalid size");
            }

            return static_cast<std::size_t>(result);
        }
        catch (...)
        {
            throw std::invalid_argument(
                "Invalid value for --size: " + value
            );
        }
    }

    int parseIterations(const std::string& value)
    {
        try
        {
            std::size_t position = 0;
            const auto result = std::stoi(value, &position);

            if (position != value.size() || result <= 0)
            {
                throw std::invalid_argument("invalid iterations");
            }

            return result;
        }
        catch (...)
        {
            throw std::invalid_argument(
                "Invalid value for --iterations: " + value
            );
        }
    }
}

Arguments parseArguments(int argc, char* argv[])
{
    Arguments args;

    for (int i = 1; i < argc; ++i)
    {
        const std::string argument = argv[i];

        if (argument == "--help" || argument == "-h")
        {
            printHelp();
            std::exit(EXIT_SUCCESS);
        }

        if (argument == "--size" || argument == "-n")
        {
            if (i + 1 >= argc)
            {
                throw std::invalid_argument(
                    "Missing value for --size"
                );
            }

            args.size = parseSize(argv[++i]);
        }
        else if (argument == "--iterations" || argument == "-i")
        {
            if (i + 1 >= argc)
            {
                throw std::invalid_argument(
                    "Missing value for --iterations"
                );
            }

            args.iterations = parseIterations(argv[++i]);
        }
        else if (argument == "--no-verify")
        {
            args.verify = false;
        }
        else
        {
            throw std::invalid_argument(
                "Unknown argument: " + argument
            );
        }
    }

    return args;
}

void printHelp()
{
    std::cout
        << "Matrix Multiplication\n\n"
        << "Usage:\n"
        << "  matmul [options]\n\n"
        << "Options:\n"
        << "  -n, --size <N>          Matrix size (default: 1024)\n"
        << "  -i, --iterations <N>    Number of iterations (default: 1)\n"
        << "      --no-verify         Disable result verification\n"
        << "  -h, --help              Show this help message\n";
}