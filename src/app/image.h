#ifndef IMAGE_H
#define IMAGE_H

#include <opencv2/opencv.hpp>
#include <string>
//#include "glfw/glfw3.h"
#include "glad/glad.h"
#include "imgui/imgui.h"

namespace Chaf
{
	class CImage
	{
	public:
		CImage();
		void Init();

	public:
		bool Load_Image(const std::string path, const std::string name="demo");
		void Set_Display_Output();
		void Set_DIsplay_Input();
		void Set_Select();
		void Reset_Image_Size();
		void Set_Image_Size(const int width, const int height, const int set_flag=0);	//	set_flag =0: set from output_img; =1: set from display_img
		void Reset();
		void Select_Boundingbox(ImVec2 pos);

	public:
		cv::Mat input_img;
		cv::Mat output_img;
		cv::Mat select_img;

		std::vector<cv::Rect> res_boxes;
		std::vector<std::string> res_classes;
		std::vector<cv::Mat> segment_mask;
		cv::Rect select_box;
		std::vector<cv::Point> points;

	public:
		bool is_detected = false;
		bool is_segment = false;
		bool is_pose_estimation = false;

	public:
		int width();
		int height();
		int texture();
		std::string name();

	private:
		bool LoadTexture(GLuint* out_texture);
		unsigned char* Mat_to_Byte(const cv::Mat img);

	private:
		std::string img_path;
		std::string img_name;
		GLuint img_gui_texture;

		cv::Mat display_img;
		
		int img_width = 0;
		int img_height = 0;

		
	};
}

#endif