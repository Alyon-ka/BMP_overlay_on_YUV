#include "Image.h"

#include <exception>
#include <thread>

//Y = (0.257 * R) + (0.504 * G) + (0.098 * B) + 16
//Cr = V = (0.439 * R) - (0.368 * G) - (0.071 * B) + 128
//Cb = U = -(0.148 * R) - (0.291 * G) + (0.439 * B) + 128

void rgb_to_yuv(uint8_t* y, uint8_t* u, uint8_t* v, uint8_t* rgb) {

	*y = static_cast<uint8_t>((0.257 * rgb[0]) + (0.504 * rgb[1]) + (0.098 * rgb[2]) + 16);
	*u = static_cast<uint8_t>((0.439 * rgb[0]) - (0.368 * rgb[1]) - (0.071 * rgb[2]) + 128);
	*v = static_cast<uint8_t>(-(0.148 * rgb[0]) - (0.291 * rgb[1]) + (0.439 * rgb[2]) + 128);
}

void rgb_to_y(uint8_t* y, const uint8_t* rgb) {

	*y = static_cast<uint8_t>((0.257 * rgb[0]) + (0.504 * rgb[1]) + (0.098 * rgb[2]) + 16);
}

void rgb_to_uv(uint8_t* u, uint8_t* v, const uint8_t* rgb) {

	*u = static_cast<uint8_t>((0.439 * rgb[0]) - (0.368 * rgb[1]) - (0.071 * rgb[2]) + 128);
	*v = static_cast<uint8_t>(-(0.148 * rgb[0]) - (0.291 * rgb[1]) + (0.439 * rgb[2]) + 128);
}

void Image::convert_rgb_to_yuv(Image& yuv_img, bool mt)
{
	size_t y_sz = img_height_ * img_width_;
	size_t u_sz = y_sz / 4;

	yuv_img.img_depth_ = img_depth_;
	yuv_img.img_height_ = img_height_;
	yuv_img.img_width_ = img_width_;
	yuv_img.img_data_.resize(y_sz + u_sz * 2);

	uint8_t *y = yuv_img.img_data_.data();
	uint8_t *u = y + y_sz;
	uint8_t *v = u + u_sz;

	auto convert_to_y = [this, &y] {

		for (uint8_t *it = img_data_.data(), *ite = img_data_.data() + img_data_.size(); it != ite; it += 3, ++y) {

			rgb_to_y(y, it);
		}
	};

	auto convert_to_uv = [this, &u, &v]() {

		for (size_t i = 0; i < img_height_; i += 2) {

			for (size_t j = 0; j < img_width_; j += 2) {

				uint8_t *rgb_ptr = &img_data_[(i *img_width_ + j) * 3];
				rgb_to_uv(u, v, rgb_ptr);
				++u;
				++v;
			}
		}
	};

	if (mt) {

		std::thread t(convert_to_uv);
		convert_to_y();
	
		t.join();
		return;
	}

	convert_to_y();
	convert_to_uv();
}

void Image::overlay_yuv_img(const Image &overlay_img)
{
	if (img_data_.size() < overlay_img.img_data_.size()) {
		throw std::runtime_error("overlay image size is larger than current image size");
	}

	size_t y_sz = img_height_ * img_width_;
	size_t u_sz = y_sz / 4;

	size_t overlay_img_y_sz = overlay_img.img_height_ * overlay_img.img_width_;
	size_t overlay_img_u_sz = overlay_img_y_sz / 4;

	int u_wigth = img_width_ / 2;
	int u_height = img_height_ / 2;

	int overlay_img_u_wigth = overlay_img.img_width_ / 2;
	int overlay_img_u_height = overlay_img.img_height_ / 2;

	uint8_t *ptr_y = img_data_.data();
	uint8_t *ptr_u = ptr_y + y_sz;
	uint8_t *ptr_v = ptr_u + u_sz;

	const uint8_t *overlay_img_ptr_y = overlay_img.img_data_.data();
	const uint8_t *overlay_img_ptr_u = overlay_img_ptr_y + overlay_img_y_sz;
	const uint8_t *overlay_img_ptr_v = overlay_img_ptr_u + overlay_img_u_sz;
	
	for (auto overlay_img_ptr_y_end = overlay_img_ptr_y + overlay_img_y_sz;
		overlay_img_ptr_y != overlay_img_ptr_y_end;
		ptr_y += img_width_, overlay_img_ptr_y += overlay_img.img_width_
		) 
	{
		memcpy(ptr_y, overlay_img_ptr_y, overlay_img.img_width_);
	}

	for (auto overlay_img_ptr_u_end = overlay_img_ptr_v;
		overlay_img_ptr_u != overlay_img_ptr_u_end; 
		ptr_u += u_wigth, ptr_v += u_wigth, 
		overlay_img_ptr_u += overlay_img_u_wigth, overlay_img_ptr_v += overlay_img_u_wigth)
	{
		memcpy(ptr_u, overlay_img_ptr_u, overlay_img_u_wigth);
		memcpy(ptr_v, overlay_img_ptr_v, overlay_img_u_wigth);
	}

}