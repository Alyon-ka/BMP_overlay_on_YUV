#pragma once
#include "Image.h"
#include <string>

class YUVFile {

public:

	YUVFile(size_t width, size_t height, size_t depth_);

	void overlay_video(const std::string & in_file_name, const std::string & out_file_name, const Image & overlay_img);

private:

	int width_;
	int height_;
	int depth_;
};