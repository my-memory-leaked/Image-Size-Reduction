#include "pch.h"
#include "CppRLEEncode.h"

void CppRLEEncode(const u8* input, u8* output, u32 &outputSize, u32& currentElement, u32& X, const u32 width, const u32 Y)
{
	bool uncompressed = true;

	while (uncompressed)
	{
		u8 repetitionCounter = 0;
		const u8 pixel = input[currentElement]; // 1 pixel

		while (currentElement < width + Y * width && pixel == input[currentElement] && repetitionCounter < 255)
		{
			repetitionCounter++;
			currentElement++;
			X++;
		}

		if (currentElement == width + Y * width)	//if end of width
			uncompressed = false;

		output[outputSize++] = repetitionCounter;
		output[outputSize++] = pixel;
	}
}
