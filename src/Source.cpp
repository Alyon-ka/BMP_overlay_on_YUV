#include "Image.h"
#include "bmpfile.h"
#include "yuvfile.h"
#include <iostream>

#include <chrono> 

int main(int argc, char **argv) {
	
	if (argc < 5) { // We expect 5 arguments: the program name, the BMP image source, the YUV  source and YUV resolution (width, height)
		std::cerr << "We expect 5 arguments. Get: " << argv[0] << "arguments" << std::endl;
		return 1;
	}

	std::string bmp_img_fname = argv[1];
	std::string yuv_videio_fname = argv[2];

	std::string str = argv[3];
	int yuv_img_width = stoi(str);
	str = argv[4];
	int yuv_img_height = stoi(str);
	
	BMPFile bmp;
	Image img, yuv_img, yuv_img_1;

	bmp.read_image_as_rgb(bmp_img_fname, img);

	auto start_time = std::chrono::system_clock::now();
	img.convert_rgb_to_yuv(yuv_img);
	auto end_time = std::chrono::system_clock::now();
	std::chrono::duration<double> diff = end_time - start_time;
	std::cout << "time: " << diff.count() << std::endl;

	start_time = std::chrono::system_clock::now();
	img.convert_rgb_to_yuv(yuv_img_1, true);
	end_time = std::chrono::system_clock::now();
	diff = end_time - start_time;
	std::cout << "time mt: " << diff.count() << std::endl;

	std::system("PAUSE");

	YUVFile yuv(yuv_img_width, yuv_img_height, 12);

	yuv.overlay_video(yuv_videio_fname, "yuv_video_with_overlay.yuv", yuv_img);

	std::system("PAUSE");
	return 0;
}



