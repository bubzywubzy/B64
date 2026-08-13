#include "../include/base64.hpp"

#include <iostream>
#include <optional>
#include <string>

#include <unistd.h>

namespace
{
    constexpr char kVersion[] = "1.0.0";

    void print_help()
    {
        std::cout
            << "Usage: b64 [OPTION] [INPUT]\n\n"
            << "Options:\n"
            << "  -e, --encode    Encode input using Base64 (default).\n"
            << "  -d, --decode    Decode Base64 input.\n"
            << "  -h, --help      Display usage information.\n"
            << "  -v, --version   Display the program version.\n\n"
            << "If INPUT is omitted, input is read from stdin, so b64\n"
            << "composes with Unix pipelines, e.g.:\n\n"
            << "  echo -n \"Hello, World!\" | b64 --encode\n";
    }
}

int main(int argc, char** argv)
{
    bool encode_mode = false;
    bool decode_mode = false;
    std::optional<std::string> positional_input;

    for (int i = 1; i < argc; ++i)
    {
        const std::string arg = argv[i];

        if (arg == "-e" || arg == "--encode")
        {
            encode_mode = true;
        }
        else if (arg == "-d" || arg == "--decode")
        {
            decode_mode = true;
        }
        else if (arg == "-h" || arg == "--help")
        {
            print_help();
            return 0;
        }
        else if (arg == "-v" || arg == "--version")
        {
            std::cout << "b64 " << kVersion << '\n';
            return 0;
        }
        else if (!arg.empty() && arg[0] == '-' && arg != "-")
        {
            std::cerr << "Unknown option: " << arg << '\n';
            return 1;
        }
        else if (!positional_input.has_value())
        {
            // The first non-option argument is treated as the input,
            // matching the README's `b64 -e "Hello, World!"` usage.
            positional_input = arg;
        }
        else
        {
            std::cerr << "Error: unexpected extra argument: " << arg << '\n';
            return 1;
        }
    }

    // No mode specified -> default to encode.
    if (!encode_mode && !decode_mode)
    {
        encode_mode = true;
    }

    if (encode_mode && decode_mode)
    {
        std::cerr << "Error: choose either encode or decode.\n";
        return 1;
    }

    std::string input;

    if (positional_input.has_value())
    {
        input = *positional_input;
    }
    else
    {
        // No INPUT argument: fall back to stdin. Only prompt when stdin
        // is an interactive terminal, so piped input stays clean, e.g.:
        //   echo -n "Hello, World!" | b64 --encode
        if (isatty(fileno(stdin)))
        {
            std::cout << "Input: ";
        }

        std::getline(std::cin, input);
    }

    try
    {
        std::cout << (encode_mode ? encode(input) : decode(input)) << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}