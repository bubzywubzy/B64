#pragma once

#include <string>

/**
 * @file base64.hpp
 * @brief Standard RFC 4648 Base64 encoding and decoding.
 */

/**
 * Encode arbitrary binary data as standard RFC 4648 Base64.
 *
 * @param input Bytes to encode. May contain embedded NUL bytes.
 * @return      Base64 text, padded with '=' as required by RFC 4648.
 * @throws std::length_error if the encoded output would overflow
 *         std::string::max_size().
 */
std::string encode(const std::string& input);

/**
 * Decode standard RFC 4648 Base64 text back into raw bytes.
 *
 * Decoding is strict: the input must be canonical Base64 as produced
 * by encode() above.
 *
 * @param input Base64 text to decode.
 * @return      The decoded bytes.
 * @throws std::invalid_argument if the input:
 *           - has a length that is not a multiple of 4
 *           - contains a character outside the Base64 alphabet
 *           - places '=' padding anywhere but the final one or two
 *             characters
 *           - has non-canonical (non-zero) padding bits
 */
std::string decode(const std::string& input);