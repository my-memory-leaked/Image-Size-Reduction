#include <bmp.h>
#include "../CppRLEEncode/CppRLEEncode.h"
using namespace kp;

#define ASM

BitMap::BitMap(const std::string& filePath)
{
	if (filePath.length() == 0)
		throw "No file selected!";

	AsmRLEEncode = (RLEEncode)GetProcAddress(LoadLibrary(TEXT(ASM_DLL_RLE_LIBRARY_PATH)),
		RLE_COMPRESSION_FUNCTION_NAME_DLL);

	changeDestFileName(filePath);
	if (read(filePath) && compressRLE())
		writeCompressedBMP();
}

BitMap::~BitMap()
{
	if (p_pixelByteData)
		free(p_pixelByteData);
	if (p_compressedData)
		free(p_compressedData);
	if (p_pallet)
		free(p_pallet);
}

// check transparent images
bool BitMap::read(const std::string& file_path)
{
	FILE* ptrFileInput;
	if (fopen_s(&ptrFileInput, file_path.c_str(), READ_BINARY))
	{
		throw "Input file opening failed";
		return false;
	}

	if (!ptrFileInput)	// When there is no file or bad filename
	{
		throw "Memory allocation error!";
		return false;
	}

	// Reading file header
	fread( &p_bmpFileHeader.file_type, sizeof(u16), 1, ptrFileInput );
	fread( &p_bmpFileHeader.file_size, sizeof(u32), 1, ptrFileInput );
	fread( &p_bmpFileHeader.reserved_data1, sizeof(u16), 1, ptrFileInput );
	fread( &p_bmpFileHeader.reserved_data2, sizeof(u16), 1, ptrFileInput );
	fread( &p_bmpFileHeader.data_offset, sizeof(u32), 1, ptrFileInput );

	// Reading file info header
	fread( &p_bmpInfoHeader.size, sizeof(u32), 1, ptrFileInput );
	fread( &p_bmpInfoHeader.width, sizeof(i32), 1, ptrFileInput );
	fread( &p_bmpInfoHeader.height, sizeof(i32), 1, ptrFileInput );
	fread( &p_bmpInfoHeader.planes, sizeof(u16), 1, ptrFileInput );
	fread( &p_bmpInfoHeader.bit_count, sizeof(u16), 1, ptrFileInput );
	fread( &p_bmpInfoHeader.compression, sizeof(u32), 1, ptrFileInput );
	fread( &p_bmpInfoHeader.size_image, sizeof(u32), 1, ptrFileInput );
	fread( &p_bmpInfoHeader.x_pixels_per_meter, sizeof(i32), 1, ptrFileInput );
	fread( &p_bmpInfoHeader.y_pixels_per_meter, sizeof(i32), 1, ptrFileInput );
	fread( &p_bmpInfoHeader.colors_used, sizeof(u32), 1, ptrFileInput );
	fread( &p_bmpInfoHeader.colors_important, sizeof(u32), 1, ptrFileInput );

	// BMP has to be uncompressed and 8bit
	if (p_bmpInfoHeader.compression != 0 || p_bmpInfoHeader.bit_count != 8 ||
		p_bmpInfoHeader.size != 0x28 || p_bmpFileHeader.file_type != 0x4d42) // others than windows bitmaps are not supported
	{
		throw "Wrong file selected";
		return false;
	}

	// Skipping data from the beginning to the start offset
	p_pallet = (u8*)malloc(p_bmpFileHeader.data_offset - SIZE_OF_FILE_HEADER);
	if (!p_pallet)
	{
		throw "Memory allocation error!";
		return false;
	}

	u64 it;
	for (it = 0; it < (p_bmpFileHeader.data_offset - SIZE_OF_FILE_HEADER) / PREFIX_KILO; it++)
	{
		// Read first kB
		fread(p_pallet + it * PREFIX_KILO, 1, PREFIX_KILO, ptrFileInput);
	}
	// Read if there is some rest of it
	fread(p_pallet + it * PREFIX_KILO, 1, (p_bmpFileHeader.data_offset - SIZE_OF_FILE_HEADER) % PREFIX_KILO, ptrFileInput);

	// Second way of getting pallet
	/*p_palletSize = bmp_file_header.data_offset - (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	p_pallet = (u8*)malloc(p_palletSize);
	fseek(ptrFileInput, p_palletSize, SEEK_SET);
	fread(&p_pallet, sizeof(u8), p_palletSize - 1, ptrFileInput);
	*/

	// Calculate size of pixels
	p_pixelSize = p_bmpFileHeader.file_size - p_bmpFileHeader.data_offset;

	// Set file ptr to the beggining of pixel data
	fseek(ptrFileInput, p_bmpFileHeader.data_offset , SEEK_SET );
	p_pixelByteData = (u8*)malloc(p_pixelSize * sizeof(u8));

	// Read all of the pixels
	fread(p_pixelByteData, sizeof(u8), p_pixelSize, ptrFileInput);

	fclose(ptrFileInput);
	return true;
}


std::string BitMap::getFileDestination()
{
    return p_fileDestination;
}


void BitMap::changeDestFileName( const std::string& file_path )
{
	const auto dot_ptr = file_path.find_last_of( '.' );
	p_fileDestination = file_path;
	p_fileDestination.insert( dot_ptr, DESTINATION_FILE_ADDON );
}

void BitMap::writeCompressedBMP()
{
	// Update the header info
	p_bmpInfoHeader.size_image = static_cast<u32>(p_totalCompressedDataSize);
	p_bmpFileHeader.file_size = static_cast<u32>(p_bmpFileHeader.data_offset + p_totalCompressedDataSize);
	p_bmpInfoHeader.compression = 0x0001;

	// Open output file
	FILE* ptrFileOutput;
	if (fopen_s(&ptrFileOutput, p_fileDestination.c_str(), WRITE_BINARY))
		throw std::invalid_argument("Output file creation failed");

	// Write file header
	fwrite(&p_bmpFileHeader.file_type, sizeof(u16), 1, ptrFileOutput);
	fwrite(&p_bmpFileHeader.file_size, sizeof(u32), 1, ptrFileOutput);
	fwrite(&p_bmpFileHeader.reserved_data1, sizeof(u16), 1, ptrFileOutput);
	fwrite(&p_bmpFileHeader.reserved_data2, sizeof(u16), 1, ptrFileOutput);
	fwrite(&p_bmpFileHeader.data_offset, sizeof(u32), 1, ptrFileOutput);

	// Write file info header
	fwrite(&p_bmpInfoHeader.size, sizeof(u32), 1, ptrFileOutput);
	fwrite(&p_bmpInfoHeader.width, sizeof(i32), 1, ptrFileOutput);
	fwrite(&p_bmpInfoHeader.height, sizeof(i32), 1, ptrFileOutput);
	fwrite(&p_bmpInfoHeader.planes, sizeof(u16), 1, ptrFileOutput);
	fwrite(&p_bmpInfoHeader.bit_count, sizeof(u16), 1, ptrFileOutput);
	fwrite(&p_bmpInfoHeader.compression, sizeof(u32), 1, ptrFileOutput);
	fwrite(&p_bmpInfoHeader.size_image, sizeof(u32), 1, ptrFileOutput);
	fwrite(&p_bmpInfoHeader.x_pixels_per_meter, sizeof(i32), 1, ptrFileOutput);
	fwrite(&p_bmpInfoHeader.y_pixels_per_meter, sizeof(i32), 1, ptrFileOutput);
	fwrite(&p_bmpInfoHeader.colors_used, sizeof(u32), 1, ptrFileOutput);
	fwrite(&p_bmpInfoHeader.colors_important, sizeof(u32), 1, ptrFileOutput);

	fwrite(p_pallet, p_bmpFileHeader.data_offset - SIZE_OF_FILE_HEADER, 1, ptrFileOutput);

	//// 2 ways writing compressed data
	//u32 totalLinesWritten = 0;
	//for (int Y = 0; Y < p_bmpInfoHeader.height; Y++) {
	//	fwrite(out + totalLinesWritten, LineLengths[Y], 1, ptrFileOutput); // write compressed stream
	//	totalLinesWritten += LineLengths[Y];
	//}

	// Write compressed pixels
	if (p_compressedData != nullptr)
		fwrite(p_compressedData, sizeof(u8), p_totalCompressedDataSize, ptrFileOutput);
	
	fclose(ptrFileOutput);
}

bool BitMap::compressRLE()
{
	p_compressedData = (u8*)malloc(p_pixelSize * sizeof(u8) * 2);
	if (!p_compressedData)
	{
		throw "Memory allocation error!";
		return false; 
	}

	const u8* inp = p_pixelByteData;	// input buffer
	u8* out = p_compressedData;	// output buffer

	p_totalCompressedDataSize = 0;	// total size of compressed data
	u64 currentElement = 0;

	// Padding will be added later
	//const int paddingAmount = (4 - (p_bmpInfoHeader.width * 3) % 4) % 4;
	//u32 padding = p_bmpInfoHeader.width % 4;
	//if (padding != 0) 
	//	padding = 4 - padding;

	for (u64 Y = 0; Y < p_bmpInfoHeader.height; Y++)
	{
		for (u64 X = 0; X < p_bmpInfoHeader.width; X++)
		{

#ifdef ASM
			
			AsmRLEEncode(inp, out, p_totalCompressedDataSize, currentElement, X, p_bmpInfoHeader.width, Y);
#else
			CppRLEEncode(inp, out, p_totalCompressedDataSize, currentElement, X, p_bmpInfoHeader.width, Y);
#endif

		}
		out[p_totalCompressedDataSize++] = 0x00;

		if (Y == p_bmpInfoHeader.height - 1)
		{
			out[p_totalCompressedDataSize++] = 0x01;
		}
		else
		{
			out[p_totalCompressedDataSize++] = 0x00;
		}	// EOL 
		
	}
	return true;
}
