#pragma once
#include "Image.h"
#include <string>

#pragma pack(push, 1)

struct BITMAPFILEHEADER
{
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;
};

struct BITMAPINFOHEADER
{
	uint32_t biSize;
	int biWidth;
	int biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
};

#pragma pack(pop)

class BMPFile {

public:
	BMPFile() {}
	void read_image_as_rgb(const std::string& file_name, Image& img);
		
private:
	
	BITMAPFILEHEADER bmp_file_header_;
	BITMAPINFOHEADER bmp_info_header_;
	void convert_bmp_to_rgb(Image& img) const;
};
