#pragma once

#include "globals.h"
//#include "CPPRLEEncode.h"

class RLECompression
{
public:
	void RLE_Compress( u8* input, u32 inputSize, u8* output );
};

