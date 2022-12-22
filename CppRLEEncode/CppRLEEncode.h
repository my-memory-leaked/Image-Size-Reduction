#pragma once

#include <stdint.h>

// Typedefs  kind of ones used in STM library
typedef uint8_t			u8;
typedef uint16_t		u16;
typedef uint32_t		u32;
typedef uint64_t		u64;
typedef int8_t			i8;
typedef int16_t			i16;
typedef int32_t			i32;
typedef int64_t			i64;

/// Encodes precised size of input using RLE lose-less algorithm
/// 
/// <param name="input"></param> Starting position pointer
/// <param name="output"></param> Output pointer to write encoded elements
/// 
extern "C" __declspec(dllexport) void CppRLEEncode(const u8* input, u8* output, u64& outputSize, u64& currentElement, u64& X, u64 width, u64 Y);
