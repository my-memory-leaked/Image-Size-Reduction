#pragma once

#include "globals.h"
//#include "CPPRLEEncode.h"

#include "qlibrary.h"


class RLECompression
{
public:

	u8* compressedData;

	void RLE_Compress( u8* input, u32 input_size);


};

