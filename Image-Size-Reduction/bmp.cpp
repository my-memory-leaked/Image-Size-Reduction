#include <bmp.h>


BitMap::BitMap(const std::string file_path)
{
	changeDestFileName(file_path);
	read(file_path);

	write();
}

BitMap::~BitMap() {

	// TODO check if it is allocated and then delete from heap
	if (pixel_data)
	{
	free(pixel_data);

	}
	if (m_pallet)
		free(m_pallet);
}

// check transparent images
bool BitMap::read(const std::string file_path) {
	FILE* ptrFileInput;

	
	ptrFileInput = fopen(file_path.c_str(), "rb");	// read binary

	if (!ptrFileInput)	// When there is no file
		return false;

	// TODO check if it is windows file
	// also add if statements below

	//fread( &bmp_file_header, sizeof(BITMAPFILEHEADER), 1, file_ptr );	 // dont work idk why
	fread( &bmp_file_header.file_type, sizeof(u16), 1, ptrFileInput );
	fread( &bmp_file_header.file_size, sizeof(u32), 1, ptrFileInput );
	fread( &bmp_file_header.reserved_data1, sizeof(u16), 1, ptrFileInput );
	fread( &bmp_file_header.reserved_data2, sizeof(u16), 1, ptrFileInput );
	fread( &bmp_file_header.data_offset, sizeof(u32), 1, ptrFileInput );

	//fread( &bmp_info_header, sizeof(BITMAPINFOHEADER), 1, file_ptr );	// dont work idk why
	fread( &bmp_info_header.size, sizeof(u32), 1, ptrFileInput );
	fread( &bmp_info_header.width, sizeof(i32), 1, ptrFileInput );
	fread( &bmp_info_header.height, sizeof(i32), 1, ptrFileInput );
	fread( &bmp_info_header.planes, sizeof(u16), 1, ptrFileInput );
	fread( &bmp_info_header.bit_count, sizeof(u16), 1, ptrFileInput );
	fread( &bmp_info_header.compression, sizeof(u32), 1, ptrFileInput );
	fread( &bmp_info_header.size_image, sizeof(u32), 1, ptrFileInput );
	fread( &bmp_info_header.x_pixels_per_meter, sizeof(i32), 1, ptrFileInput );
	fread( &bmp_info_header.y_pixels_per_meter, sizeof(i32), 1, ptrFileInput );
	fread( &bmp_info_header.colors_used, sizeof(u32), 1, ptrFileInput );
	fread( &bmp_info_header.colors_important, sizeof(u32), 1, ptrFileInput );

	
	//if (bmp_info_header.compression != 0)	// bmp has to be uncompressed
	//	return false;

	//if (bmp_info_header.bit_count != 8)	// there is only one 8 bit conversion available
	//	return false;


	//m_palletSize = bmp_file_header.data_offset - (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
	//m_pallet = (u8*)malloc(m_palletSize);
	//fseek(ptrFileInput, m_palletSize, SEEK_SET);

	/*fread(&m_pallet, sizeof(u8), m_palletSize - 1, ptrFileInput);*/

	m_pixelSize = (bmp_file_header.file_size ) - bmp_file_header.data_offset;

	//m_pixelSize = bmp_file_header.data_offset;
	fseek(ptrFileInput, bmp_file_header.data_offset , SEEK_SET );

	pixel_data = (u8*)calloc( m_pixelSize, sizeof(u8) );	// better to use calloc
	//pixel_data = (u8*)malloc(m_pixelSize * sizeof(u8));
	//m_pixels.resize(m_pixelSize);

	fread(pixel_data, sizeof(u8), m_pixelSize, ptrFileInput);



	fclose(ptrFileInput);
}


/// <summary> Adds _resized string to destination file
/// 
/// </summary>
/// <param name="file_path"></param>
void BitMap::changeDestFileName( const std::string file_path ) {
	auto dot_ptr = file_path.find_last_of( '.' );
	file_destination = file_path;
	file_destination.insert( dot_ptr, DESTINATION_FILE_ADDON );
}


bool BitMap::write() {
	FILE* ptrFileOutput = fopen( file_destination.c_str(), "wb");

	//fread( &bmp_file_header, sizeof(BITMAPFILEHEADER), 1, file_ptr );	 // dont work idk why
	fwrite(&bmp_file_header.file_type, sizeof(u16), 1, ptrFileOutput);
	fwrite(&bmp_file_header.file_size, sizeof(u32), 1, ptrFileOutput);
	fwrite(&bmp_file_header.reserved_data1, sizeof(u16), 1, ptrFileOutput);
	fwrite(&bmp_file_header.reserved_data2, sizeof(u16), 1, ptrFileOutput);
	fwrite(&bmp_file_header.data_offset, sizeof(u32), 1, ptrFileOutput);

	//fread( &bmp_info_header, sizeof(BITMAPINFOHEADER), 1, file_ptr );	// dont work idk why
	fwrite(&bmp_info_header.size, sizeof(u32), 1, ptrFileOutput);
	fwrite(&bmp_info_header.width, sizeof(i32), 1, ptrFileOutput);
	fwrite(&bmp_info_header.height, sizeof(i32), 1, ptrFileOutput);
	fwrite(&bmp_info_header.planes, sizeof(u16), 1, ptrFileOutput);
	fwrite(&bmp_info_header.bit_count, sizeof(u16), 1, ptrFileOutput);
	fwrite(&bmp_info_header.compression, sizeof(u32), 1, ptrFileOutput);
	fwrite(&bmp_info_header.size_image, sizeof(u32), 1, ptrFileOutput);
	fwrite(&bmp_info_header.x_pixels_per_meter, sizeof(i32), 1, ptrFileOutput);
	fwrite(&bmp_info_header.y_pixels_per_meter, sizeof(i32), 1, ptrFileOutput);
	fwrite(&bmp_info_header.colors_used, sizeof(u32), 1, ptrFileOutput);
	fwrite(&bmp_info_header.colors_important, sizeof(u32), 1, ptrFileOutput);


	//fwrite(&bmp_file_header, sizeof(BITMAPFILEHEADER), 1, ptrFileOutput);
	//fwrite(&bmp_info_header, sizeof(BITMAPINFOHEADER), 1, ptrFileOutput);


	//fwrite(&m_pallet, m_palletSize, 1, ptrFileOutput);
	fwrite(pixel_data, sizeof(u8), m_pixelSize, ptrFileOutput);
	

	fclose(ptrFileOutput);
	return true;

}
 

void BitMap::compressRLE()
{
	// TODO
	//RLECompression::RLE_Compress(&bmp_file_header.data_offset,  );		

}
