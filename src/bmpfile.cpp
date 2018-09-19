#include "bmpfile.h"
#include <algorithm>

void BMPFile::read_image_as_rgb(const std::string& file_name, Image& img)
{
	std::ifstream ifs;
	ifs.open(file_name, std::ifstream::binary);

	bmp_file_header_;
	bmp_info_header_;

	ifs.read((char*)(&bmp_file_header_), sizeof(BITMAPFILEHEADER));
	ifs.read((char*)(&bmp_info_header_), sizeof(BITMAPINFOHEADER));

	size_t data_size_ = (size_t)(bmp_info_header_.biWidth * bmp_info_header_.biHeight * bmp_info_header_.biBitCount / 8);

	img.img_data_.resize(data_size_);
	img.img_depth_ = bmp_info_header_.biBitCount;
	img.img_height_ = bmp_info_header_.biHeight;
	img.img_width_ = bmp_info_header_.biWidth;

	ifs.read((char*)(img.img_data_.data()), data_size_);
	
	convert_bmp_to_rgb(img);

	ifs.close();
}

void BMPFile::convert_bmp_to_rgb(Image& img) const
{
	Image new_img = img;
	new_img.img_data_.clear();
	new_img.img_data_.resize(img.img_data_.size());

	for (int i = 0; i < img.img_height_; ++i) {
		for (int j = 0; j < img.img_width_; ++j)
		{
			int n_bmp = (j + img.img_width_ * (img.img_height_ - i - 1)) * 3;
					
			auto ptr_data = new_img.at(i, j, 24);
			ptr_data[0] = img.img_data_[n_bmp];
			ptr_data[1] = img.img_data_[n_bmp + 1];
			ptr_data[2] = img.img_data_[n_bmp + 2];
		}
	}

	img.img_data_.swap(new_img.img_data_);
}
