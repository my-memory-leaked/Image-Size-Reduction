#include "pch.h"
#include "CppRLEEncode.h"
#include <stdlib.h>

// TODO chceck if couter is not grater than 255
void RLEEncode( u8* input, u8* output, u32 &outputSize, u16 width ) {

	u8* inp = input;	// input buffer
	u8* out = output;	// output buffer

	u8 pixel;	// current pixel used in comparison
	u32 currentElement = 0;
	u32 repetitionCounter = 0;	// Counts repeating elements
	u32 p_totalCompressedDataSize = outputSize;	// total size of compressed data
	u16 lineLength = 0;
	bool uncompressed;
	
	for (u32 X = 0; X < width; X++)
	{
		uncompressed = true;

		while (uncompressed)
		{
			repetitionCounter = 0;
			pixel = inp[currentElement]; // 1 pixel
			while ( (currentElement < width)
					&& (pixel == inp[currentElement]) && (repetitionCounter < 255))
			{
				repetitionCounter++;
				currentElement++;
				X++;
			}
			if (currentElement == width )	//if end of width
				uncompressed = false;

			out[p_totalCompressedDataSize++] = repetitionCounter;
			out[p_totalCompressedDataSize++] = pixel;

			lineLength += 2;
		}

	}

	outputSize = p_totalCompressedDataSize;

}   // RLEEncode( u8* input, u32 inputSize, u8* output )
