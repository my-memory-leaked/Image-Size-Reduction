#include <bmp.h>


BitMap::BitMap(const std::string file_path)
{
	changeDestFileName(file_path);
	read(file_path);
}

BitMap::~BitMap() {

	// TODO check if it is allocated and then delete from heap
	if (!pixel_data)
	{

	}
	free(pixel_data);

}


bool BitMap::read(const std::string file_path) {
	FILE* file_ptr;
	u32 pixel_size;	// size of pixels in bytes

	file_ptr = fopen(file_path.c_str(), "rb");	// read binary

	if (!file_ptr)	// When there is no file
		return false;

	// TODO check if it is windows file
	// also add if statements below

	//fread( &bmp_file_header, sizeof(BITMAPFILEHEADER), 1, file_ptr );	 // dont work idk why
	fread( &bmp_file_header.file_type, sizeof(u16), 1, file_ptr );
	fread( &bmp_file_header.file_size, sizeof(u32), 1, file_ptr );
	fread( &bmp_file_header.reserved_data1, sizeof(u16), 1, file_ptr );
	fread( &bmp_file_header.reserved_data2, sizeof(u16), 1, file_ptr );
	fread( &bmp_file_header.data_offset, sizeof(u32), 1, file_ptr );

	//fread( &bmp_info_header, sizeof(BITMAPINFOHEADER), 1, file_ptr );	// dont work idk why
	fread( &bmp_info_header.size, sizeof(u32), 1, file_ptr );
	fread( &bmp_info_header.width, sizeof(i32), 1, file_ptr );
	fread( &bmp_info_header.height, sizeof(i32), 1, file_ptr );
	fread( &bmp_info_header.planes, sizeof(u16), 1, file_ptr );
	fread( &bmp_info_header.bit_count, sizeof(u16), 1, file_ptr );
	fread( &bmp_info_header.compression, sizeof(u32), 1, file_ptr );
	fread( &bmp_info_header.size_image, sizeof(u32), 1, file_ptr );
	fread( &bmp_info_header.x_pixels_per_meter, sizeof(i32), 1, file_ptr );
	fread( &bmp_info_header.y_pixels_per_meter, sizeof(i32), 1, file_ptr );
	fread( &bmp_info_header.colors_used, sizeof(u32), 1, file_ptr );
	fread( &bmp_info_header.colors_important, sizeof(u32), 1, file_ptr );

	if (bmp_info_header.compression != 0)	// bmp has to be uncompressed
		return false;

	if (bmp_info_header.bit_count != 8)	// there is only one 8 bit conversion available
		return false;

	pixel_size = bmp_file_header.file_size - bmp_file_header.data_offset;

	fseek( file_ptr, bmp_file_header.data_offset , SEEK_SET );
	pixel_data = (u8*)calloc( pixel_size, sizeof(u8) );	// better to use calloc
	fread(pixel_data, pixel_size, 1, file_ptr);

}


/// <summary> Adds _resized string to destination file
/// 
/// </summary>
/// <param name="file_path"></param>
void BitMap::changeDestFileName( const std::string file_path ) {
	auto dot_ptr = file_path.find_last_of(".");
	file_destination = file_path;
	file_destination.insert( dot_ptr, DESTINATION_FILE_ADDON );
}


//bool BitMap::write(const std::string file_path) {


void BitMap::compressRLE()
{
	// TODO
	//RLECompression::RLE_Compress(&bmp_file_header.data_offset,  );		

}
