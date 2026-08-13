#include "../include/base64.hpp"

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    bool encode_mode = false;
    bool decode_mode = false;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

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
            std::cout
                << "Usage: b64 [OPTION]\n\n"
                << "Options:\n"
                << "  -e, --encode    Encode text\n"
                << "  -d, --decode    Decode Base64\n"
                << "  -h, --help      Show this help\n";

            return 0;
        }
        else
        {
            std::cerr << "Unknown option: " << arg << '\n';
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

    std::cout << "Input: ";
    std::getline(std::cin, input);

    try
    {
        if (encode_mode)
        {
            std::cout << "Output: " << encode(input) << '\n';
        }
        else
        {
            std::cout << "Output: " << decode(input) << '\n';
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}