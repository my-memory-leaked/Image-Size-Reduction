# Image Compression with RLE (Run-Length Encoding)

This repository contains a C++ project that utilizes the Qt library for building a graphical user interface (GUI) application to compress 8-bit BMP images using the Run-Length Encoding (RLE) algorithm. The project includes two dynamic-link libraries (DLLs): one written in C++ with the OpenMP library for parallelization, and the other written in assembly language.

# Contributor
- With the invaluable help of Wiktor Merta, who provided extensive contributions, particularly in the realm of assembly programming.

## Features

- Graphical user interface for easy interaction.
- RLE compression algorithm specifically designed for 8-bit BMP images.
- Two DLLs for performance optimization:
  - The C++ DLL uses the OpenMP library to parallelize the compression process and leverage multiple CPU cores.
  - The assembly DLL provides optimized routines for faster execution.
- Supports compressing multiple BMP images in batch mode.
- Outputs the compressed images as a new file with the `.rle` extension.

## Prerequisites

To build and run this project, ensure you have the following installed:

- Visual studio with MSVC compiler.
- Qt development environment.
- OpenMP library for parallelization.

## Acknowledgments

- The RLE compression algorithm used in this project is based on [Run-Length Encoding (RLE)](https://en.wikipedia.org/wiki/Run-length_encoding).
- The Qt library provides an excellent framework for building cross-platform applications.
- The OpenMP library enables parallel execution to boost performance.
- Using assembly language definitely slows down the program, but the gained knowledge is more valuable.

## License

This project is licensed under the [MIT License](LICENSE). Feel free to use, modify, and distribute the code as per the terms of the license.
