#include <bmp.h>
using namespace kp;

BitMap::BitMap(const std::string& file_path)
{

	AsmRLEEncode = (RLEEncode)GetProcAddress(LoadLibrary(TEXT(ASM_DLL_RLE_LIBRARY_PATH)),
	                                                     RLE_COMPRESSION_FUNCTION_NAME_DLL);

	changeDestFileName(file_path);
	read(file_path);

	compressRLE();
	writeCompressedBMP();
}

BitMap::~BitMap() {

	// TODO check if it is allocated and then delete from heap
	if (p_pixelByteData)
	{
	free(p_pixelByteData);

	}

	if (p_compressedData)
		free(p_compressedData);


	if (p_pallet)
		free(p_pallet);

}

// check transparent images
bool BitMap::read(const std::string& file_path)
{

	FILE* ptrFileInput;
	if(fopen_s(&ptrFileInput, file_path.c_str(), READ_BINARY))
		throw std::invalid_argument("Input file opening failed");

	if (!ptrFileInput)	// When there is no file or bad filename
		throw std::bad_alloc();

	// TODO check if it is windows file
	// also add if statements below

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
	//if (bmp_info_header.compression != 0 || bmp_info_header.bit_count != 8 || bmp_info_header.bit_count != 4)
	//{
	//	throw std::bad_alloc();
	//	return false;
	//}

	if (p_bmpInfoHeader.size != 0x28 || p_bmpFileHeader.file_type != 0x4d42)
		return false;               // others than windows bitmaps are not supported
	if (p_bmpInfoHeader.bit_count == 24)
		return false;               // 24 bit bitmaps cannot be compressed
	if (p_bmpInfoHeader.compression != 0)
		return false;               // only uncompressed bitmaps may be compressed


	// Skipping data from the beginning to the start offset
	p_pallet = (u8*)malloc(p_bmpFileHeader.data_offset - SIZE_OF_FILE_HEADER);
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
	p_bmpInfoHeader.size_image = p_totalCompressedDataSize;
	p_bmpFileHeader.file_size = p_bmpFileHeader.data_offset + p_totalCompressedDataSize;
	p_bmpInfoHeader.compression = 0x0001;

	// Open output file
	FILE* ptrFileOutput;
	if(fopen_s(&ptrFileOutput, p_fileDestination.c_str(), WRITE_BINARY))
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
 

void BitMap::compressRLE()
{
	p_compressedData = (u8*)malloc(p_pixelSize * sizeof(u8) * 2);

	const u8* inp = p_pixelByteData;	// input buffer
	u8* out = p_compressedData;	// output buffer
	const auto lineLengths = new u16[p_bmpInfoHeader.height];	// Array of compressed line lengths

	p_totalCompressedDataSize = 0;	// total size of compressed data
	u32 currentElement = 0;

	// Padding will be added later
	/*const int paddingAmount = (4 - (p_bmpInfoHeader.width * 3) % 4) % 4;
	u32 padding = p_bmpInfoHeader.width % 4;
	if (padding != 0) 
		padding = 4 - padding;*/

	for (u32 Y = 0; Y < p_bmpInfoHeader.height; Y++)
	{
		lineLengths[Y] = 0;
		for (u32 X = 0; X < p_bmpInfoHeader.width; X++)
		{

#ifdef ASM
			AsmRLEEncode(inp, out, lineLengths, p_totalCompressedDataSize, currentElement, p_bmpInfoHeader.width, Y, X);
#else
			CppRLEEncode(inp, out, lineLengths, p_totalCompressedDataSize, currentElement, p_bmpInfoHeader.width, Y, X);
#endif

			//bool uncompressed = true;

			//while (uncompressed)
			//{
			//	u8 repetitionCounter = 0;
			//	const u8 pixel = inp[currentElement]; // 1 pixel
			//	while ( currentElement < p_bmpInfoHeader.width + Y * p_bmpInfoHeader.width 
			//			 && pixel == inp[currentElement] && repetitionCounter < 255)
			//	{
			//		repetitionCounter++;
			//		currentElement++;
			//		X++;
			//	}
			//	if (currentElement == (p_bmpInfoHeader.width + (Y * p_bmpInfoHeader.width)) )	//if end of width
			//		uncompressed = false;

			//	out[p_totalCompressedDataSize++] = repetitionCounter;
			//	out[p_totalCompressedDataSize++] = pixel;

			//	lineLengths[Y] += 2;
			//}
		}
		out[p_totalCompressedDataSize++] = 0x00;

		if (Y == p_bmpInfoHeader.height - 1)
		{
			out[p_totalCompressedDataSize++] = 0x01;
		}
		else
		{
			out[p_totalCompressedDataSize++] = 0x00;
		}
		lineLengths[Y] += 2;
		// EOL 

	}

	//// TODO Add compression rate later
	////printf(stdout, "Compression ratio = %f%%\n", 100.0 - (image->startOffset + total) * 100.0 / image->size);

	delete[] lineLengths;
}
