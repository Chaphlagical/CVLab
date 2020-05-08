/*#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/shape_utils.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <vector>*/
#include "dnn.h"


namespace Chaf
{
	class CYolo: 
		public CDNN
	{
	public:
		CYolo();

	public:
		void Load_Model();
		bool Get_Result(std::vector<cv::Rect>& boxes, std::vector<std::string>& classes);
		void Update(cv::Mat& input, cv::Mat& output);

	private:
		std::vector<std::string> Get_Output_Name();
		void postprocess(cv::Mat input_img);
		void postprocess(cv::Mat input_img, cv::Mat& output_img);
		void set_net_input(cv::Mat& input_img);

	 private:
		std::vector<cv::Rect> res_boxes;
		std::vector<std::string> res_classes;
		
	};
}