#ifndef IMAGE_H
#define IMAGE_H

#include <opencv2/opencv.hpp>
#include "yolo.h"
#include <string>
//#include "glfw/glfw3.h"
#include "glad/glad.h"

namespace Chaf
{
	class CImage
	{
	public:
		CImage();
		void Init();

	public:
		void Load_Image(const std::string path, const std::string name="demo");
		void Object_Detection();

	public:
		int width();
		int height();
		int texture();
		std::string name();

	private:
		bool LoadTextureFromFile(const char* filename, GLuint* out_texture);
		unsigned char* Mat_to_Byte(const cv::Mat img);

	private:
		Chaf::CYolo yolo_detection;

	private:
		std::string img_path;
		std::string img_name;
		GLuint img_gui_texture;
		cv::Mat img;
		int img_width = 0;
		int img_height = 0;



	};
}

#endif