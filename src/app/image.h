#include <opencv2/opencv.hpp>
#include "yolo.h"
#include <string>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glfw/glfw3.h"

namespace Chaf
{
	class CImage
	{
	public:
		CImage();
		void Init();

	public:
		void Load_Image(const std::string path);
		void Object_Detection();

	private:
		Chaf::CYolo yolo_detection;

	private:
		std::string img_path;
		GLuint img_gui_texture;
		int img_width = 0;
		int img_height = 0;


	};
}