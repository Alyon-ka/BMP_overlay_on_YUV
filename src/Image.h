#pragma once

#include <fstream>
#include <vector>

class Image {

public:

	Image() {}

	uint8_t * at(int i, int j, int img_depth = 8)
	{
		return &img_data_[(i *img_width_ + j) * img_depth / 8];
	}

	int img_depth_;
	size_t img_width_;
	size_t img_height_;
	std::vector<uint8_t> img_data_;

	void convert_rgb_to_yuv(Image& yuv_img, bool mt = false);
	void overlay_yuv_img(const Image& overlay_img);
};
