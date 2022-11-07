#pragma once

#include "globals.h"
#include "CPPRLEEncode.h"

#include "qlibrary.h"


class RLECompression
{
public:
	static void RLE_Compress( u8* input, u32 input_size, u8* output );
};

