#pragma once

#include <stdint.h>

// Typedefs  kind of ones used in STM library
typedef uint8_t			u8;
typedef uint16_t		u16;
typedef uint32_t		u32;
typedef int8_t			i8;
typedef int16_t			i16;
typedef int32_t			i32;

/// Encodes precised size of input using RLE lose-less algorithm
/// 
/// <param name="input"></param> Starting position pointer
/// <param name="output"></param> Output pointer to write encoded elements
/// 
extern "C" __declspec(dllexport) void CppRLEEncode(const u8* input, u8* output, u16* lineLengths, u32 &outputSize, u32& currentElement, u32 width, u32 Y, u32 X);
