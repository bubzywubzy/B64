#include "../include/base64.hpp"
 
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdexcept>
 
namespace
{
    constexpr char kAlphabet[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
 
    constexpr char kPad = '=';
 
    // Maps an ASCII byte to its 6-bit Base64 value, or -1 if the byte
    // is not part of the Base64 alphabet. Built at compile time.
    constexpr std::array<std::int8_t, 256> make_decode_table()
    {
        std::array<std::int8_t, 256> table{};
 
        for (auto& value : table)
        {
            value = -1;
        }
 
        for (std::size_t i = 0; i < 64; ++i)
        {
            table[static_cast<unsigned char>(kAlphabet[i])] =
                static_cast<std::int8_t>(i);
        }
 
        return table;
    }
 
    constexpr auto kDecodeTable = make_decode_table();
 
    // Number of Base64 characters needed to encode `input_size` bytes:
    // ceil(input_size / 3) * 4, computed without overflowing.
    [[nodiscard]] constexpr std::size_t encoded_size(std::size_t input_size)
    {
        constexpr std::size_t max_input =
            (std::numeric_limits<std::size_t>::max() / 4) * 3 - 2;
 
        if (input_size > max_input)
        {
            throw std::length_error(
                "base64::encode: input too large for output to be "
                "represented");
        }
 
        return ((input_size + 2) / 3) * 4;
    }
 
    // Upper bound on decoded byte count for a validated, `input_size`-byte
    // Base64 string (before subtracting any '=' padding).
    [[nodiscard]] constexpr std::size_t max_decoded_size(std::size_t input_size)
    {
        return (input_size / 4) * 3;
    }
}
 
std::string encode(const std::string& input)
{
    std::string output;
    output.reserve(encoded_size(input.size()));
 
    std::size_t i = 0;
 
    // Consume input three bytes (24 bits) at a time; each group becomes
    // four 6-bit Base64 characters.
    for (; i + 3 <= input.size(); i += 3)
    {
        const std::uint32_t chunk =
            (static_cast<std::uint32_t>(static_cast<unsigned char>(input[i])) << 16) |
            (static_cast<std::uint32_t>(static_cast<unsigned char>(input[i + 1])) << 8) |
             static_cast<std::uint32_t>(static_cast<unsigned char>(input[i + 2]));
 
        output.push_back(kAlphabet[(chunk >> 18) & 0x3F]);
        output.push_back(kAlphabet[(chunk >> 12) & 0x3F]);
        output.push_back(kAlphabet[(chunk >> 6) & 0x3F]);
        output.push_back(kAlphabet[chunk & 0x3F]);
    }
 
    // Handle the final one or two leftover bytes, padding with '='.
    switch (input.size() - i)
    {
        case 1:
        {
            const auto b0 = static_cast<std::uint32_t>(
                static_cast<unsigned char>(input[i]));
 
            output.push_back(kAlphabet[(b0 >> 2) & 0x3F]);
            output.push_back(kAlphabet[(b0 << 4) & 0x3F]);
            output.push_back(kPad);
            output.push_back(kPad);
            break;
        }
        case 2:
        {
            const auto b0 = static_cast<std::uint32_t>(
                static_cast<unsigned char>(input[i]));
            const auto b1 = static_cast<std::uint32_t>(
                static_cast<unsigned char>(input[i + 1]));
 
            output.push_back(kAlphabet[(b0 >> 2) & 0x3F]);
            output.push_back(kAlphabet[((b0 << 4) | (b1 >> 4)) & 0x3F]);
            output.push_back(kAlphabet[(b1 << 2) & 0x3F]);
            output.push_back(kPad);
            break;
        }
        default:
            // Nothing left over; input length was a multiple of 3.
            break;
    }
 
    return output;
}
 
std::string decode(const std::string& input)
{
    if (input.empty())
    {
        return {};
    }
 
    if (input.size() % 4 != 0)
    {
        throw std::invalid_argument(
            "base64::decode: length must be a multiple of 4");
    }
 
    // Count trailing '=' padding (0, 1, or 2 characters).
    std::size_t pad_count = 0;
    if (input[input.size() - 1] == kPad)
    {
        ++pad_count;
        if (input[input.size() - 2] == kPad)
        {
            ++pad_count;
        }
    }
 
    const std::size_t data_len = input.size() - pad_count;
 
    // '=' must appear only as trailing padding, never mid-string.
    for (std::size_t i = 0; i < data_len; ++i)
    {
        if (input[i] == kPad)
        {
            throw std::invalid_argument(
                "base64::decode: '=' padding character in invalid "
                "position");
        }
    }
 
    // Every remaining character must be a valid Base64 symbol.
    for (std::size_t i = 0; i < data_len; ++i)
    {
        const auto c = static_cast<unsigned char>(input[i]);
        if (kDecodeTable[c] < 0)
        {
            throw std::invalid_argument(
                "base64::decode: invalid Base64 character");
        }
    }
 
    // Reject non-canonical encodings where the unused low bits of the
    // final symbol(s) are not zero (RFC 4648 section 3.5).
    if (pad_count == 1)
    {
        const auto c = static_cast<unsigned char>(input[input.size() - 2]);
        if ((kDecodeTable[c] & 0x03) != 0)
        {
            throw std::invalid_argument(
                "base64::decode: non-zero padding bits");
        }
    }
    else if (pad_count == 2)
    {
        const auto c = static_cast<unsigned char>(input[input.size() - 3]);
        if ((kDecodeTable[c] & 0x0F) != 0)
        {
            throw std::invalid_argument(
                "base64::decode: non-zero padding bits");
        }
    }
 
    std::string output;
    output.reserve(max_decoded_size(input.size()) - pad_count);
 
    // Decode complete 4-character groups; the last group may include
    // padding, in which case it contributes fewer than 3 output bytes.
    for (std::size_t i = 0; i < input.size(); i += 4)
    {
        const bool has_c2 = input[i + 2] != kPad;
        const bool has_c3 = input[i + 3] != kPad;
 
        const std::uint32_t chunk =
            (static_cast<std::uint32_t>(kDecodeTable[static_cast<unsigned char>(input[i])]) << 18) |
            (static_cast<std::uint32_t>(kDecodeTable[static_cast<unsigned char>(input[i + 1])]) << 12) |
            (has_c2 ? static_cast<std::uint32_t>(kDecodeTable[static_cast<unsigned char>(input[i + 2])]) << 6 : 0U) |
            (has_c3 ? static_cast<std::uint32_t>(kDecodeTable[static_cast<unsigned char>(input[i + 3])]) : 0U);
 
        output.push_back(static_cast<char>((chunk >> 16) & 0xFF));
 
        if (has_c2)
        {
            output.push_back(static_cast<char>((chunk >> 8) & 0xFF));
        }
        if (has_c3)
        {
            output.push_back(static_cast<char>(chunk & 0xFF));
        }
    }
 
    return output;
}
