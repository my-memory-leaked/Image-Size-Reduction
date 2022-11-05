#pragma once

#include <stdint.h>

#define CPP_LIBRARY_API __declspec(dllimport)

// Typedefs  kind of ones used in STM library
typedef uint8_t			u8;
typedef uint16_t		u16;
typedef uint32_t		u32;

/// Encodes precised size of input using RLE loseless algorithm
/// 
/// <param name="input"></param> Starting position poiter
/// <param name="inputSize"></param> Size of inpot to be encoded
/// <param name="output"></param> Output pointer to write encoded elements
/// 
extern "C" CPP_LIBRARY_API void RLEEncode( u8 * input, u32 inputSize, u8 * output );
