#include "yuvfile.h"

YUVFile::YUVFile(size_t width, size_t height, size_t depth_)
	: width_(width)
	, height_(height)
	, depth_(depth_)
{}

void YUVFile::overlay_video(const std::string & in_file_name, const std::string & out_file_name, const Image & overlay_img)
{
	std::ifstream ifs;
	ifs.open(in_file_name, std::ifstream::binary);

	std::ofstream ofs;
	ofs.open(out_file_name, std::ofstream::binary);

	Image img;
	img.img_width_ = width_;
	img.img_height_ = height_;
	img.img_depth_ = depth_;
	img.img_data_.resize((size_t)(width_ * height_ * 3 / 2));

	while (true) {
		ifs.read((char*)img.img_data_.data(), img.img_data_.size());
		if (ifs.eof()) break;

		img.overlay_yuv_img(overlay_img);
		ofs.write((char*)img.img_data_.data(), img.img_data_.size());
	}

	ifs.close();
	ofs.close();
}
