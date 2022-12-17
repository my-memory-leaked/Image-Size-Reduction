#pragma once

#include <stdint.h>
#include <windows.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#define ASM

#define  DESTINATION_FILE_ADDON             "_compressed"
#define  RLE_COMPRESSION_FUNCTION_NAME_DLL  "RLEEncode"

#ifdef _DEBUG
#define  CPP_DLL_RLE_LIBRARY_PATH           "../x64/Debug/CppRLEEncode.dll"
#define  ASM_DLL_RLE_LIBRARY_PATH           "../x64/Debug/AsmRLEEncode.dll"
#else
#define  CPP_DLL_RLE_LIBRARY_PATH           "../x64/Release/CppRLEEncode.dll"
#define  ASM_DLL_RLE_LIBRARY_PATH           "../x64/Release/AsmRLEEncode.dll"
#endif

/// File reading attributes
#define READ_BINARY			"rb"
#define WRITE_BINARY		"wb"






