# B64 — Base64 Encoder/Decoder

A lightweight command-line Base64 encoder and decoder written in modern C++.

`b64` provides RFC 4648-compliant Base64 encoding and decoding directly from the terminal, with a simple Unix-style command-line interface.

## Features

* Encode arbitrary text to Base64.
* Decode Base64 back to its original representation.
* RFC 4648 standard Base64 alphabet.
* Standard `=` padding support.
* Command-line interface with short and long options.
* Explicit encode/decode modes.
* Non-zero exit status on invalid input or operational errors.
* Designed for Unix-like systems.
* Minimal runtime dependencies.
* Built with a conventional `Makefile`.

## Installation

Clone the repository:

```bash
git clone https://github.com/bubzywubzy/b64.git
cd b64
```

Build:

```bash
make
```

Install:

```bash
make install
```

The executable will typically be installed to:

```text
/usr/local/bin/b64
```

Verify the installation:

```bash
b64 --help
```

## Usage

```text
b64 [OPTIONS] [INPUT]
```

### Options

| Option            | Description                  |
| ----------------- | ---------------------------- |
| `-e`, `--encode`  | Encode input using Base64.   |
| `-d`, `--decode`  | Decode Base64 input.         |
| `-h`, `--help`    | Display usage information.   |
| `-v`, `--version` | Display the program version. |

If neither `--encode` nor `--decode` is specified, encoding is used by default.

## Examples

### Encode a string

```bash
b64 -e "Hello, World!"
```

Output:

```text
SGVsbG8sIFdvcmxkIQ==
```

### Decode a string

```bash
b64 -d "SGVsbG8sIFdvcmxkIQ=="
```

Output:

```text
Hello, World!
```

### Using stdin

`b64` can be used as part of a Unix pipeline:

```bash
echo -n "Hello, World!" | b64
```

Decode piped input:

```bash
echo "SGVsbG8sIFdvcmxkIQ==" | b64 --decode
```

This allows `b64` to compose naturally with other Unix utilities.

For example:

```bash
cat file.txt | b64 --encode
```

and:

```bash
cat encoded.txt | b64 --decode
```

## Base64

Base64 is a binary-to-text encoding scheme that represents binary data using a 64-character alphabet.

The standard RFC 4648 alphabet consists of:

```text
ABCDEFGHIJKLMNOPQRSTUVWXYZ
abcdefghijklmnopqrstuvwxyz
0123456789+/
```

The `=` character is used as padding when the input length is not evenly divisible by three bytes.

For example:

```text
Hello
```

becomes:

```text
SGVsbG8=
```

Base64 is an **encoding**, not encryption. It provides no confidentiality or cryptographic protection.

## Standards

B64 implements the standard Base64 alphabet and encoding rules defined by:

* RFC 4648 — The Base16, Base32, and Base64 Data Encodings

The implementation is intended to produce interoperable standard Base64 rather than URL-safe Base64.

## Error Handling

The decoder validates Base64 input before producing decoded output.

Invalid input may include:

* Characters outside the Base64 alphabet.
* Invalid padding.
* Incorrect input length.
* Malformed encoded data.

Errors are reported through the command-line interface and result in a non-zero process exit status.

This makes `b64` suitable for use in shell scripts where command success or failure can be evaluated using `$?`.

Example:

```bash
b64 --decode "invalid input"
echo $?
```

## Build Requirements

### Required

* C++ compiler with modern C++ support
* GNU Make

For example:

```bash
g++
make
```

The project is designed for Linux and other Unix-like environments.

## Building from Source

Clean previous build artifacts:

```bash
make clean
```

Build the project:

```bash
make
```

Install:

```bash
make install
```

Uninstall:

```bash
make uninstall
```

## Project Structure

```text
b64/
├── include/
│   └── base64.hpp
├── src/
│   ├── base64.cpp
│   └── main.cpp
├── Makefile
├── LICENSE
└── README.md
```

The project separates the Base64 implementation from the command-line interface:

* `include/` — Public C++ declarations.
* `src/` — Implementation and CLI entry point.
* `Makefile` — Build and installation targets.

## Design Goals

B64 is intentionally small.

The goal is to provide a focused implementation that demonstrates how a real command-line utility can be structured in C++ without introducing unnecessary dependencies or abstraction.

The project emphasizes:

1. Correct Base64 encoding and decoding.
2. Input validation.
3. Predictable command-line behavior.
4. Unix pipeline compatibility.
5. Clear separation between library and CLI code.
6. Conventional C++ project organization.

## Security Considerations

Base64 should not be confused with encryption.

Anyone who has Base64-encoded data can decode it without a key:

```bash
b64 --decode "c2VjcmV0"
```

Therefore, Base64 should **never** be used as a mechanism for protecting passwords, credentials, secrets, or other sensitive information.

For confidentiality, use an appropriate cryptographic primitive instead.

## Testing

A useful test suite should verify both normal operation and malformed input.

Example round-trip test:

```bash
echo -n "Hello, World!" | b64 --encode | b64 --decode
```

Expected output:

```text
Hello, World!
```

Additional tests should cover:

* Empty input.
* One-byte input.
* Two-byte input.
* Three-byte input.
* Inputs requiring padding.
* Inputs without padding where supported.
* Binary data.
* Invalid Base64 characters.
* Invalid padding.
* Large inputs.

## Contributing

Contributions are welcome.

Before submitting a change:

1. Build the project successfully.
2. Run the test suite.
3. Verify existing CLI behavior has not regressed.
4. Add tests for new functionality.
5. Keep changes focused and minimal.

For bugs or feature requests, open an issue with:

* A description of the problem.
* Steps to reproduce it.
* Expected behavior.
* Actual behavior.
* Relevant command output.

Pull requests should explain what was changed and why.

## License

See [`LICENSE`](LICENSE) for the project's license terms.

## Author

**bubzywubzy**

GitHub:

```text
https://github.com/bubzywubzy/b64
```
