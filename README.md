# B64- Base 64 Encoder/Decoder

This is a simple CLI tool written in C++ that allows you to encode and decode strings to and from Base64 format. It provides a convenient way to handle Base64 encoding and decoding operations directly from the terminal.

## Features

- Encode strings to Base64 format.
- Decode Base64 strings back to their original format.
- Follows RFC 4648 standard for Base64 encoding and decoding.

### Installation

```bash
make
make install
```

## Usage

```bash
b64 [options] \\ either -e or -d default: -e
    -e, --encode    Encode the input string to Base64 format.
    -d, --decode    Decode the input Base64 string back to its original format.
    -h, --help      Display this help message.
```

### Contribution

Contributions are welcome! If you find any issues or have suggestions for improvements, please feel free to open an issue or submit a pull request.
